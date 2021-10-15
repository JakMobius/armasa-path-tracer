#pragma once

#include "../../scene/scene.hpp"
#include "../../scene/materials/material_lambertian.hpp"
#include "../../scene/materials/material_lambertian_light.hpp"
#include "../example_view.hpp"
#include "../../scene/build_cube.hpp"
#include "../../scene/build_rect.hpp"
#include "../../scene/materials/material_metal.hpp"
#include "../../scene/materials/material_dielectric.hpp"
#include "../../scene/hittables/hittable_sphere.hpp"
#include "../../scene/build_model.hpp"
#include "../../scene/hittables/hittable_triangle.hpp"
#include "../../utils/vec2.hpp"

class PlaygroundView : public ExampleView {
public:

    void init_window() override {
        create_window(1240, 720, 1);
    }

    void init_camera() override {
        ExampleView::init_camera();

        camera->set_position({-15, 0, 3.25});
        camera->set_focus_distance(5.5);
        camera->matrix = Matrix4f::rotation_y_matrix(-0.41) * camera->matrix;
        controller->set_camera_speed(1);
    }

    void init_drawer() override {
        ExampleView::init_drawer();

        drawer->get_post_processing_program()->set_brightness(1.0);
        drawer->get_post_processing_program()->set_gamma(1);
        drawer->set_static_frame_substeps(10);
        drawer->set_dynamic_frame_substeps(1);
        drawer->set_max_reflections(20);

        frames_logging = 10;
    }

    void create_light(const Vec2f& position, const Vec3f& color) {
        Material* m_glass = new MaterialDielectric({1.0, 1.0, 1.0}, 1.3, -1, 1);
        Material* m_light = new MaterialLambertianLight(color);
        build_model(scene->get_root_hittable(), "resources/models/bamboo-outer.obj",
            Matrix4f::rotation_x_matrix(M_PI / 2) *
            Matrix4f::scale_matrix(0.3, 0.3, 0.3) *
            Matrix4f::translation_matrix(position[0], position[1], -5),
        m_glass);
        build_model(scene->get_root_hittable(), "resources/models/bamboo-inner.obj",
                    Matrix4f::rotation_x_matrix(M_PI / 2) *
                            Matrix4f::scale_matrix(0.3, 0.3, 0.3) *
                Matrix4f::translation_matrix(position[0], position[1], -5),
        m_light);
    }

    void init_scene() override {
        ExampleView::init_scene();

        Material *m_red   = new MaterialLambertian({1, 0, 0});
        Material *m_green   = new MaterialLambertian({0, 1, 0});
        Material* m_white = new MaterialLambertian({1, 1, 1});
        Material* m_light = new MaterialLambertianLight({2, 2, 2});
        Material* m_glass = new MaterialDielectric({0.5, 1, 1}, 1.3, -1, 0);

        float lamp_size = 1.6;

        build_cube(scene->get_root_hittable(), Matrix4f::scale_matrix(10000, 10000, 10), (Material*[6]){
            nullptr, nullptr,
            nullptr, nullptr,
            nullptr, m_white
        });

        scene->get_root_hittable()->add_children(new HittableSphere({10*4, 10*4, 7*3}, 7*4.2, m_light));
//        scene->get_root_hittable()->add_children(new HittableSphere({10, 15, 10}, 7, m_light));
        scene->get_root_hittable()->add_children(new HittableSphere({-10*4, 10*4, 7*3}, 7*4.2, m_light));
//        scene->get_root_hittable()->add_children(new HittableSphere({-9, 9, 10}, 7, m_light));

//        Vec3f colors[] = {
//            {0.35, 1.55, 0.85},
//            {1.341, 0.054, 0.87},
//            {1.725, 1.066, 0.439},
//            {0.439, 1.066, 1.725},
//            {1.360, 1.282, 0}
//        };


//        for(int i = -30; i < 10; i ++) {
//            float x = (float)i;
//            float y = sin((float)i) * 0.8;
//
//            float nx = -x * 0.6 - y + 8;
//            float ny = y - x - 1.7;
//
//            create_light({nx, ny}, colors[(i + 30) % 5]);

//            create_light({nx - 26, ny}, colors[(i + 30) % 5]);
//            create_light({nx - 27, ny + 1}, colors[(i + 30) % 5]);
//            create_light({nx - 28, ny + 2}, colors[(i + 30) % 5]);
//            create_light({nx - 29, ny + 3}, colors[(i + 30) % 5]);
//            create_light({nx, -ny}, colors[(i + 1) % 5]);
//        }
    }
};