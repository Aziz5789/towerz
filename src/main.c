#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>
#include <math.h>
#include <stdio.h>

static const int WINDOW_HEIGHT = 600;
static const int WINDOW_WIDTH = 800;

static const char *VERTEX_SHADER_SRC =
    "#version 330 core\n"
    "uniform vec2 u_offset;\n"
    "layout(location = 0) in vec3 a_pos;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(a_pos.xy + u_offset, a_pos.z, 1.0);\n"
    "}\n";

static const char *FRAGMENT_SHADER_SRC = "#version 330 core\n"
                                         "uniform vec4 u_color;\n"
                                         "out vec4 frag_color;\n"
                                         "\n"
                                         "void main() {\n"
                                         "    frag_color = u_color;\n"
                                         "}\n";

static const float RECT_VERTICES[] = {
    -0.4f, -0.15f, 0.0f, // 0: bottom left
    0.4f,  -0.15f, 0.0f, // 1: bottom right
    0.4f,  0.15f,  0.0f, // 2: top right
    -0.4f, 0.15f,  0.0f, // 3: top left
};

static const unsigned int RECT_INDICES[] = {
    0, 1, 2, // triangle 1
    0, 2, 3, // triangle 2
};

static GLuint compile_shader(GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        fprintf(stderr, "shader compile failed: \n%s\n", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint create_program(const char *vs_src, const char *fs_src) {

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_src);
    if (vs == 0) {
        return 0;
    }

    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_src);
    if (fs == 0) {
        glDeleteShader(vs);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (!result) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        fprintf(stderr, "shader link failed: \n%s\n", log);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    int exit_code = 1;
    SDL_Window *window = NULL;
    SDL_GLContext gl = NULL;
    GLuint program = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint vao = 0;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        goto clean_up;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

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

    program = create_program(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);

    if (program == 0) {
        fprintf(stderr, "create_program failed!\n");
        goto clean_up;
    }

    GLint color_loc = glGetUniformLocation(program, "u_color");

    if (color_loc == -1) {
        fprintf(stderr, "location of u_color not found!\n");
        goto clean_up;
    }

    GLint offset_loc = glGetUniformLocation(program, "u_offset");

    if (offset_loc == -1) {
        fprintf(stderr, "location of u_offset not found!\n");
        goto clean_up;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RECT_VERTICES), RECT_VERTICES, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECT_INDICES), RECT_INDICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    printf("OpenGL %s\n", (const char *)glGetString(GL_VERSION));

    // sky blue color
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

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

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        Uint64 ticks = SDL_GetTicks();
        glUniform4f(color_loc, 1.0f, 0.3f, (ticks % 1000) / 1000.0f, 1.0f);
        glUniform2f(offset_loc, 0.5f * sinf(ticks / 1000.0f), 0);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

        SDL_GL_SwapWindow(window);
    }
    exit_code = 0;

clean_up:
    if (vao) {
        glDeleteVertexArrays(1, &vao);
    }
    if (ebo) {
        glDeleteBuffers(1, &ebo);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
    if (program) {
        glDeleteProgram(program);
    }
    if (gl) {
        SDL_GL_DestroyContext(gl);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    return exit_code;
}