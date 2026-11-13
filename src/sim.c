#include "sim.h"
#include <math.h>
#include <stdbool.h>

static const float PIVOT_X = 0.0f;
static const float CRANE_GAP = 2.5f;
static const float ROPE_HEIGHT = 6.0f;
static const float RADIUS_X = 1.25f;   // side-to-side width
static const float RADIUS_Y = 0.50f;   // up-and-down depth
static const float LOOP_SPEED = 1.25f; // radians per second
static const float GRAVITY = -9.0f;    // gravity
static const float STRAIGHTEN_SPEED = 2.5f;
static const float SCROLL_SPEED = 3.0f;

static Box block_box(const Block *b) {
    Box box = {b->x, b->y, BLOCK_HALF, BLOCK_HALF};
    return box;
}

static float box_top(Box b) { return b.y + b.half_h; }

static bool block_hits(Box falling, Box target) {

    float bottom = falling.y - falling.half_h;
    float top = box_top(target);

    float dx = fabsf(falling.x - target.x);
    float max_dx = falling.half_w + target.half_w;

    return dx < max_dx && bottom <= top;
}

static Box landing_target(const Sim *sim) {
    if (sim->tower_count == 0) {
        return sim->platform;
    } else {
        return block_box(&sim->tower[sim->tower_count - 1]);
    }
}

static void settle_block(Block *b, float surface_top) {
    b->y = surface_top + BLOCK_HALF;
    b->vy = 0.0f;
    b->angle = 0.0f;
    b->state = BLOCK_LANDED;
}

static void place_on_crane(Sim *sim) {
    float phase = LOOP_SPEED * sim->time;

    float hang_y = sim->scroll_y + CRANE_GAP;
    float pivot_y = hang_y + ROPE_HEIGHT;

    float x = PIVOT_X + RADIUS_X * sinf(phase);
    float y = hang_y - RADIUS_Y * cosf(phase);

    float angle = atan2f(x - PIVOT_X, pivot_y - y);

    sim->active_block.x = x;
    sim->active_block.y = y;
    sim->active_block.angle = angle;
}

static void spawn_block(Sim *sim) {
    sim->active_block.vy = 0.0f;
    sim->active_block.state = BLOCK_HANGING;
    place_on_crane(sim);
}

void sim_init(Sim *sim) {
    sim->time = 0.0f;

    spawn_block(sim);

    sim->tower_count = 0;

    sim->platform.x = 0.0f;
    sim->platform.y = -0.25f;
    sim->platform.half_w = 2.5f;
    sim->platform.half_h = 0.25f;

    sim->scroll_y = box_top(landing_target(sim));
}

void sim_step(Sim *sim, float dt) {

    sim->time += dt;

    sim->scroll_y += (box_top(landing_target(sim)) - sim->scroll_y) * SCROLL_SPEED * dt;

    if (sim->active_block.state == BLOCK_HANGING) {
        place_on_crane(sim);
    } else if (sim->active_block.state == BLOCK_FALLING) {
        sim->active_block.vy += GRAVITY * dt;
        sim->active_block.y += sim->active_block.vy * dt;
        sim->active_block.angle -= sim->active_block.angle * STRAIGHTEN_SPEED * dt;

        Box falling = block_box(&sim->active_block);
        Box target = landing_target(sim);

        if (block_hits(falling, target)) {
            settle_block(&sim->active_block, box_top(target));

            if (sim->tower_count < MAX_BLOCKS) {
                sim->tower[sim->tower_count] = sim->active_block;
                sim->tower_count++;
            }

            spawn_block(sim);
        } else if (sim->tower_count > 0 && block_hits(falling, sim->platform)) {
            settle_block(&sim->active_block, box_top(sim->platform));
        }
    }
}

void sim_release(Sim *sim) {
    if (sim->active_block.state == BLOCK_HANGING) {
        sim->active_block.state = BLOCK_FALLING;
        sim->active_block.vy = 0.0f;
    }
}
