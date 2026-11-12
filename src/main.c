#include "render.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>
#include <stdio.h>

static const int WINDOW_HEIGHT = 600;
static const int WINDOW_WIDTH = 800;

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    int exit_code = 1;
    SDL_Window *window = NULL;
    SDL_GLContext gl = NULL;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        goto clean_up;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Towerz", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        goto clean_up;
    }

    gl = SDL_GL_CreateContext(window);

    if (gl == NULL) {
        fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        goto clean_up;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

    if (version == 0) {
        fprintf(stderr, "gladLoadGL failed!\n");
        goto clean_up;
    }

    printf("OpenGL %s\n", (const char *)glGetString(GL_VERSION));

    if (!render_init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
        fprintf(stderr, "render_init failed!\n");
        goto clean_up;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        render_frame(SDL_GetTicks() / 1000.0f);
        SDL_GL_SwapWindow(window);
    }
    exit_code = 0;

clean_up:

    render_shutdown();

    if (gl) {
        SDL_GL_DestroyContext(gl);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    return exit_code;
}