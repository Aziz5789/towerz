#ifndef SIM_H
#define SIM_H

typedef struct {
    float x;
    float y;
    float angle;
} Block;

typedef struct {
    Block block;
    float time;
} Sim;

void sim_init(Sim *sim);
void sim_step(Sim *sim, float dt);



#endif