

#include <iostream>
#include "graphics/window.hpp"
#include "graphics/programs/triangle_program.hpp"
#include "graphics/vertex_array_object_factory.hpp"

void app() {
    Window window(500, 500, 1);
    Graphics::TriangleProgram program;

    program.addTriangle({-1, -1,
                         0, -0.3445,
                         0, 0});

    sf::Event event {};

    while(true) {
        while (window.get_sf_window()->pollEvent(event)) {
            if (event.type == sf::Event::Closed) break;
        }
        window.clear();
        program.draw();
        window.swap();
    }
}

int main() {
    app();
//    try {
//        app();
//    } catch(GLException ex) {
//        std::cout << ex.what();
//    }
}