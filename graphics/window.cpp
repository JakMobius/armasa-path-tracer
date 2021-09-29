//
// Created by Артем on 29.09.2021.
//

#include <SDL.h>
#include <OpenGL/gl3.h>
#include "window.hpp"
#include "error_handling.hpp"

static bool sdl_initialized = false;
static bool init_sdl() {
    int error = SDL_Init(SDL_INIT_VIDEO);

    if(error == -1) return false;

    sdl_initialized = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    return true;
}

Window::Window(int width, int height, int scale): width(width), height(height), scale(scale) {
    if(!sdl_initialized) init_sdl();

    handle = SDL_CreateWindow("tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(handle == nullptr) throw WindowNotCreatedException();

    context = SDL_GL_CreateContext(handle);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);

    SDL_GL_SetSwapInterval(1);
}

void Window::clear() {
    glViewport(0, 0, get_width() * get_scale(), get_height() * get_scale());
    GLException::check();

    glClear(GL_COLOR_BUFFER_BIT);
    GLException::check();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLException::check();
}

void Window::swap() {
    SDL_GL_SwapWindow(handle);
}
