#include "sim.h"
#include <math.h>

static const float PIVOT_X = 0.0f;
static const float PIVOT_Y = 10.0f;
static const float ROPE_LENGTH = 3.5f;
static const float HANG_Y = 4.0f;      // center of the loop
static const float RADIUS_X = 1.25f;   // side-to-side width
static const float RADIUS_Y = 0.50f;   // up-and-down depth
static const float LOOP_SPEED = 1.25f; // radians per second

void sim_init(Sim *sim) {
    sim->block.x = PIVOT_X;
    sim->block.y = PIVOT_Y - ROPE_LENGTH;
    sim->block.angle = 0.0f;

    sim->time = 0.0f;
}

void sim_step(Sim *sim, float dt) {

    sim->time += dt;

    float phase = LOOP_SPEED * sim->time;

    float x = PIVOT_X + RADIUS_X * sinf(phase);
    float y = HANG_Y - RADIUS_Y * cosf(phase);

    float angle = atan2f(x - PIVOT_X, PIVOT_Y - y);

    sim->block.x = x;
    sim->block.y = y;
    sim->block.angle = angle;
}