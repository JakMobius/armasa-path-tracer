//
// Created by Артем on 29.09.2021.
//

#include "window.hpp"
#include "error_handling.hpp"

Window::Window(int width, int height, int scale): width(width), height(height), scale(scale) {
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf_window = new sf::Window(sf::VideoMode(width, height, 32), "Tracer",
                                     sf::Style::Titlebar | sf::Style::Close, settings);

    sf_window->setVerticalSyncEnabled(true);
    sf_window->setFramerateLimit(60);

    sf_window->setActive(true);

    glewInit ();
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
    sf_window->display();
}

void Window::close() {
    sf_window->close();
}
