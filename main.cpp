
#include "graphics/window.hpp"
#include "graphics/programs/path_tracer_program.hpp"
#include "controls/user_controller.hpp"

void app() {
    Window window(3072, 1700, 1);
    Graphics::PathTracerProgram program;
    Graphics::Camera camera {};
    UserController controller(&camera, &window, nullptr);

    camera.set_focus_distance(2.0);
    camera.set_camera_width((float)window.get_width() / (float)window.get_height());

    program.set_camera(&camera);

    sf::Event event {};

    while(true) {
        while (window.get_sf_window()->pollEvent(event)) {
            controller.handle_event(event);
        }

        if(!window.is_open()) break;

        //program.get_camera().set_heading(program.get_camera().get_heading() + 0.01);

        controller.tick();
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