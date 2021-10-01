
#include "graphics/window.hpp"
#include "graphics/programs/path_tracer_program.hpp"
#include "controls/user_controller.hpp"
#include "scene/scene.hpp"
#include "scene/hittables/hittable_sphere.hpp"
#include "scene/hittables/hittable_list.hpp"
#include "scene/hittables/hittable_triangle.hpp"

void app() {
    Window window(2500, 1600, 1);
    Graphics::PathTracerProgram program;
    Graphics::Camera camera {};
    UserController controller(&camera, &window, nullptr);

    Scene scene;

    std::vector<std::vector<HittableSphere*>> spheres;

    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -2}, {0, 1, -2}, {1, 1, -2}));
    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -2}, {1, 0, -2}, {1, 1, -2}));

    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -2}, {1, 0, -2}, {1, 0, -1}));
    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -2}, {0, 1, -2}, {0, 1, -1}));

    scene.get_root_hittable()->add_children(new HittableTriangle({1, 1, -2}, {1, 0, -2}, {1, 0, -1}));
    scene.get_root_hittable()->add_children(new HittableTriangle({1, 1, -2}, {0, 1, -2}, {0, 1, -1}));

    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -1}, {0, 1, -1}, {1, 1, -1}));
    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -1}, {1, 0, -1}, {1, 1, -1}));

    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -1}, {1, 0, -1}, {0, 0, -2}));
    scene.get_root_hittable()->add_children(new HittableTriangle({0, 0, -1}, {0, 1, -1}, {0, 0, -2}));

    scene.get_root_hittable()->add_children(new HittableTriangle({1, 1, -1}, {1, 0, -1}, {1, 1, -2}));
    scene.get_root_hittable()->add_children(new HittableTriangle({1, 1, -1}, {0, 1, -1}, {1, 1, -2}));

    for(int i = 0; i < 3; i++) {
        spheres.push_back({});
        for(int j = 0; j < 3; j++) {
            auto new_sphere = new HittableSphere({(float) i * 5, (float) j * 5, -5}, 2);
            spheres[i].push_back(new_sphere);
            scene.get_root_hittable()->add_children(new_sphere);
        }
    }

    camera.set_focus_distance(2.0);
    camera.set_camera_width((float)window.get_width() / (float)window.get_height());

    program.set_camera(&camera);

    sf::Event event {};

    float timer = 0;

    while(true) {
        while (window.get_sf_window()->pollEvent(event)) {
            controller.handle_event(event);
        }

        if(!window.is_open()) break;

        //program.get_camera().set_heading(program.get_camera().get_heading() + 0.01);

//        for(int i = 0; i < spheres.size(); i++) {
//            for(int j = 0; j < spheres[i].size(); j++) {
//                Vec3f position = spheres[i][j]->get_position();
//                position.set_z(sin(timer + (float)(i + j) / 2));
//                spheres[i][j]->set_position(position);
//            }
//        }


        scene.render(program.get_index_buffer(), program.get_float_buffer());

        controller.tick();
        window.clear();
        program.draw();
        window.swap();

        timer += 0.05;
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