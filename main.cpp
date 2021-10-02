
#include "graphics/window.hpp"
#include "graphics/programs/path_tracer_program.hpp"
#include "controls/user_controller.hpp"
#include "scene/scene.hpp"
#include "scene/hittables/hittable_sphere.hpp"
#include "scene/hittables/hittable_list.hpp"
#include "scene/cornell_box_scene.hpp"

void app() {
    Window window(1600, 1600, 1);
    Graphics::PathTracerProgram program;
    Graphics::Camera camera {};
    UserController controller(&camera, &window, nullptr);

    Scene* scene = cornell_box_scene();
    SceneBuffer scene_buffer;
    SceneRenderer renderer(scene);

    scene_buffer.create_buffers();

    camera.set_position({-15, 0, 0});
    camera.set_focus_distance(2.0);
    camera.set_camera_width((float)window.get_width() / (float)window.get_height());

    program.set_camera(&camera);

    sf::Event event {};

    while(true) {
        while (window.get_sf_window()->pollEvent(event)) {
            controller.handle_event(event);
        }

        if(!window.is_open()) break;

        scene->get_renderer()->render(&scene_buffer);

        controller.tick();
        window.clear();
        program.draw(&scene_buffer);
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