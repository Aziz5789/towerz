#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>

bool render_init(int width, int height);

void render_frame(float time_seconds);

void render_shutdown(void);

#endif 
