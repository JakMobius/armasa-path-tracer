#pragma once

#include "../../scene/scene.hpp"
#include "../example_view.hpp"
#include "../../scene/materials/material_lambertian.hpp"
#include "../../scene/materials/material_lambertian_light.hpp"
#include "../../scene/materials/material_dielectric.hpp"
#include "../../scene/build_rect.hpp"
#include "../../scene/hittables/hittable_sphere.hpp"
#include "../../scene/build_cube.hpp"
#include "../../scene/build_model.hpp"

class TankExampleView: public ExampleView {
public:
    TankExampleView(): ExampleView() {}

    void init_window() override {
        create_window(1920, 1080, 1);
    }

    void init_camera() override {
        ExampleView::init_camera();

        camera->set_position({-15, 0, -0.2});
        camera->set_focus_distance(4);
        camera->matrix = Matrix4f::rotation_y_matrix(-0.2) * camera->matrix;
    }

    void init_drawer() override {
        ExampleView::init_drawer();

        drawer->get_post_processing_program()->set_brightness(1.0);
        drawer->get_post_processing_program()->set_gamma(1);
        drawer->set_static_frame_substeps(200);
        drawer->set_dynamic_frame_substeps(16);
        drawer->set_max_reflections(30);

        frames_logging = 1;
    }

    void init_scene() override {
        ExampleView::init_scene();

        Material *m_red   = new MaterialLambertian({1, 0, 0});
        Material *m_green   = new MaterialLambertian({0, 1, 0});
        Material* m_white = new MaterialLambertian({1, 1, 1});
        Material* m_light = new MaterialLambertianLight({2, 2, 2});
        Material* m_glass = new MaterialDielectric({0.8, 0.8, 1.0}, 1.3, -1, 0);
        Material* m_white_glass = new MaterialDielectric({1,1,1}, 1, -1, 1);
        Material* m_neon_orange = new MaterialLambertianLight({1.725, 1.066, 0.439});
        Material* m_neon_blue = new MaterialLambertianLight({0.439, 1.066, 1.725});
        Material* m_neon_green = new MaterialLambertianLight({0.439, 1.725, 1.066});

        float lamp_size = 1.6;
        float box_size = 10;

        build_rect(scene->get_root_hittable(), m_light, {0, 0, box_size / 2 - FLOAT_EPS}, {lamp_size, lamp_size, 0}, {lamp_size, -lamp_size, 0});

        build_model(scene->get_root_hittable(), "resources/models/tank.obj",
                    Matrix4f::scale_matrix(0.1, 0.1, 0.1) *
                    Matrix4f::rotation_x_matrix(M_PI / 2) *
                    Matrix4f::rotation_z_matrix(-0.9) *
                    Matrix4f::translation_matrix(0, 0, -4.5)
                , m_glass);

        scene->get_root_hittable()->add_children(new HittableSphere({2, 2, -5.5}, 1, m_light));
        scene->get_root_hittable()->add_children(new HittableSphere({0, 0, -5.5}, 1, m_light));
        scene->get_root_hittable()->add_children(new HittableSphere({4, 3, -3}, 1.6, m_neon_orange));
        scene->get_root_hittable()->add_children(new HittableSphere({3, -3, 1}, 0.9, m_neon_blue));
        scene->get_root_hittable()->add_children(new HittableSphere({-5, -5, -5}, 0.9, m_neon_green));

        scene->get_root_hittable()->add_children(new HittableSphere({4, 3, -3}, 1.7, m_white_glass));
        scene->get_root_hittable()->add_children(new HittableSphere({3, -3, 1}, 1, m_white_glass));
        scene->get_root_hittable()->add_children(new HittableSphere({-5, -5, -5}, 1, m_white_glass));

        build_cube(scene->get_root_hittable(), Matrix4f::scale_matrix(box_size, box_size, box_size), (Material*[6]){
                m_white, nullptr,
                m_red, m_green,
                m_white, m_white
        });
    }
};