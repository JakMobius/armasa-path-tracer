#pragma once

#include <SDL.h>
#include <OpenGL/gl3.h>
#include <exception>

class WindowNotCreatedException : std::exception {
    [[nodiscard]] const char * what () const noexcept override { return "Could not create SDL window"; }
};

class Window {
    SDL_Window* handle = nullptr;
    SDL_GLContext context = 0;
    int width;
    int height;
    int scale;

public:
    Window(int width, int height, int scale = 1);

    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_scale() const { return scale; }

    void swap();
    void clear();
};