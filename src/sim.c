#include "sim.h"
#include <math.h>

static const float PIVOT_X = 0.0f;
static const float PIVOT_Y = 10.0f;
static const float HANG_Y = 4.0f;      // center of the loop
static const float RADIUS_X = 1.25f;   // side-to-side width
static const float RADIUS_Y = 0.50f;   // up-and-down depth
static const float LOOP_SPEED = 1.25f; // radians per second
static const float GRAVITY = -9.0f;    // gravity
static const float STRAIGHTEN_SPEED = 2.5f;

void sim_init(Sim *sim) {
    sim->block.x = PIVOT_X;
    sim->block.y = HANG_Y - RADIUS_Y;
    sim->block.angle = 0.0f;
    sim->block.vy = 0.0f;
    sim->block.state = BLOCK_HANGING;

    sim->platform.x = 0.0f;
    sim->platform.y = -0.25f;
    sim->platform.half_w = 2.5f;
    sim->platform.half_h = 0.25f;

    sim->time = 0.0f;
}

void sim_step(Sim *sim, float dt) {

    sim->time += dt;

    if (sim->block.state == BLOCK_HANGING) {
        float phase = LOOP_SPEED * sim->time;

        float x = PIVOT_X + RADIUS_X * sinf(phase);
        float y = HANG_Y - RADIUS_Y * cosf(phase);

        float angle = atan2f(x - PIVOT_X, PIVOT_Y - y);

        sim->block.x = x;
        sim->block.y = y;
        sim->block.angle = angle;
    } else if (sim->block.state == BLOCK_FALLING) {
        sim->block.vy += GRAVITY * dt;
        sim->block.y += sim->block.vy * dt;
        sim->block.angle -= sim->block.angle * STRAIGHTEN_SPEED * dt;

        float bottom = sim->block.y - BLOCK_HALF;
        float platform_top = sim->platform.y + sim->platform.half_h;

        float dx = fabsf(sim->block.x - sim->platform.x);
        float max_dx = BLOCK_HALF + sim->platform.half_w;

        if (dx < max_dx && bottom <= platform_top) {
            sim->block.state = BLOCK_LANDED;

            sim->block.y = sim->platform.y + sim->platform.half_h + BLOCK_HALF;
            sim->block.vy = 0.0f;
            sim->block.angle = 0.0f;
        }
    }
}

void sim_release(Sim *sim) {
    if (sim->block.state == BLOCK_HANGING) {
        sim->block.state = BLOCK_FALLING;
        sim->block.vy = 0.0f;
    }
}
