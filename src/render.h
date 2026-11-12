#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include "sim.h"

bool render_init(int width, int height);

void render_frame(const Sim *sim);

void render_shutdown(void);

#endif 
