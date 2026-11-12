#include "sim.h"
#include <math.h>

void sim_init(Sim *sim) {
    sim->block.x = 0.0f;
    sim->block.y = 3.0f;
    sim->time = 0.0f;
}

void sim_step(Sim *sim, float dt) {

    sim->time += dt;
    sim->block.x = 2.0f * sinf(sim->time);
}