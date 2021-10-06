
#include <random>
#include "graphics/window.hpp"
#include "graphics/programs/path_tracer_program.hpp"
#include "controls/user_controller.hpp"
#include "scene/scene.hpp"
#include "scene/hittables/hittable_sphere.hpp"
#include "scene/hittables/hittable_list.hpp"
#include "scene/cornell_box_scene.hpp"
#include "graphics/programs/present_program.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj/tiny_obj_loader.h"
#include "scene/hittables/hittable_triangle.hpp"
#include "scene/materials/material_lambertian.hpp"
#include "scene/build_cube.hpp"
#include "scene/scene_drawer.hpp"

void app() {
    const int width = 1000;
    const int height = 1000;

    Window window(width, height, 1);
    glViewport(0, 0, width, height);

    Graphics::Camera camera;
    Graphics::PresentProgram present_program;
    UserController controller(&camera, &window, nullptr);

    Scene* scene = cornell_box_scene();
    SceneDrawer drawer(scene, &camera, width, height);

    auto* model = new HittableList();
    build_model(model, "resources/models/bobs.obj",
                Matrix4f::rotation_x_matrix(M_PI / 2) *
                Matrix4f::translation_matrix(1, 0, -1),
                new MaterialLambertian({1, 0.874, 0.768}));

    scene->get_root_hittable()->add_children(model);

    camera.set_position({-15, 0, 0});
    camera.set_focus_distance(2);
    camera.set_camera_width((float)window.get_width() / (float)window.get_height());

    present_program.set_brightness(2.0);
    present_program.set_gamma(0.4);
//
//    present_program.set_brightness(1.0);
//    present_program.set_gamma(1.0);

    sf::Event event {};

    int measured_frames = 5;
    unsigned long frame_microseconds = 0;

    while(true) {

        while (window.get_sf_window()->pollEvent(event)) {
            controller.handle_event(event);
        }

        if(!window.is_open()) break;
        controller.tick();
        if(camera.is_moved()) drawer.reset();

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        window.clear();
        drawer.draw();
        Graphics::GLFramebuffer::unbind();
        present_program.set_texture(drawer.get_output_texture());
        present_program.draw(drawer.get_frames());
        window.swap();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        frame_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        if(drawer.get_frames() % measured_frames == 0) {
            double rays_per_second = (double) measured_frames * (double) drawer.get_frame_rays() / (double)frame_microseconds * 1000000;
            std::cout << "completed frame " << drawer.get_frames() << ", "
                    << (long long)rays_per_second << " rays per second, "
                    << (long long)drawer.get_frames() * (long long) drawer.get_frame_rays() << " rays in total\n";

            frame_microseconds = 0;
        }
    }
}
int main() {
//    app();
//    try {
        app();
//    } catch(GLException &ex) {
//        std::cout << "GLException: " << ex.what() << "\n";
//    } catch(GLStringException &ex) {
//        std::cout << "GLStringException: " << ex.what() << "\n";
//    }
}