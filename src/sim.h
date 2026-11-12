#ifndef SIM_H
#define SIM_H

#define BLOCK_SIZE 1.0f
#define BLOCK_HALF (BLOCK_SIZE / 2.0f)

typedef enum{
    BLOCK_HANGING,
    BLOCK_FALLING,
    BLOCK_LANDED,
} BlockState;

typedef struct {
    float x;
    float y;
    float angle;
    float vy;
    BlockState state;
} Block;

typedef struct {
    float x;
    float y;
    float half_w;
    float half_h;
} Platform;

typedef struct {
    Block block;
    Platform platform;
    float time;
} Sim;

void sim_init(Sim *sim);
void sim_step(Sim *sim, float dt);
void sim_release(Sim *sim);


#endif