#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <exception>

class WindowNotCreatedException : std::exception {
    [[nodiscard]] const char * what () const noexcept override { return "Could not create SDL window"; }
};

class Window {

    int width;
    int height;
    int scale;

    sf::Window* sf_window;

public:
    Window(int width, int height, int scale = 1);

    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_scale() const { return scale; }

    void swap();
    void clear();

    sf::Window* get_sf_window() { return sf_window; }
};