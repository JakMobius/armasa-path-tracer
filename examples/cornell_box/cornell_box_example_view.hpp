#pragma once

#include "../../scene/scene.hpp"
#include "../../scene/materials/material_lambertian.hpp"
#include "../../scene/materials/material_lambertian_light.hpp"
#include "../example_view.hpp"
#include "../../scene/build_cube.hpp"
#include "../../scene/build_rect.hpp"

class CornellBoxExampleView : public ExampleView {
public:

    void init_window() override {
        create_window(1080, 1080, 1);
    }

    void init_camera() override {
        ExampleView::init_camera();

        camera->set_position({-15, 0, 0});
        camera->set_focus_distance(2);
    }

    void init_drawer() override {
        ExampleView::init_drawer();

        drawer->get_post_processing_program()->set_brightness(2.0);
        drawer->get_post_processing_program()->set_gamma(0.4);
        drawer->set_static_frame_substeps(4);
        drawer->set_dynamic_frame_substeps(1);
        drawer->set_max_reflections(7);
    }

    void init_scene() override {
        ExampleView::init_scene();

        Material *m_white = new MaterialLambertian({1, 1, 1});
        Material *m_red   = new MaterialLambertian({1, 0, 0});
        Material *m_green = new MaterialLambertian({0, 1, 0});
        Material *m_light = new MaterialLambertianLight({1, 1, 1});

        float lamp_size = 1.6;
        float box_size = 10;

        build_cube(scene->get_root_hittable(), Matrix4f::scale_matrix(box_size, box_size, box_size), (Material*[6]){
                m_white, nullptr,
                m_red, m_green,
                m_white, m_white
        });

        build_rect(scene->get_root_hittable(), m_light, {0, 0, box_size / 2 - FLOAT_EPS}, {lamp_size, lamp_size, 0}, {lamp_size, -lamp_size, 0});

        build_cube(scene->get_root_hittable(),
                   Matrix4f::scale_matrix(box_size * 0.2f, box_size * 0.2f, box_size * 0.75f) *
                   Matrix4f::rotation_z_matrix(M_PI * 0.3) *
                   Matrix4f::translation_matrix(box_size * 0.2f, -box_size * 0.4f, -box_size * 0.25f * 0.5f)
                , m_white);

        build_cube(scene->get_root_hittable(),
                   Matrix4f::scale_matrix(box_size * 0.2f, box_size * 0.2f, box_size * 0.25f) *
                   Matrix4f::rotation_z_matrix(M_PI * -0.3f) *
                   Matrix4f::translation_matrix(-box_size * 0.2f, box_size * 0.3f, -box_size * 0.75f * 0.5f)
                , m_white);
    }
};