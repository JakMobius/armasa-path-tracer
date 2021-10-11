#pragma once

#include "../../scene/scene.hpp"
#include "../../scene/materials/material_lambertian.hpp"
#include "../../scene/materials/material_lambertian_light.hpp"
#include "../example_view.hpp"
#include "../../scene/build_cube.hpp"
#include "../../scene/build_rect.hpp"
#include "../../scene/materials/material_dielectric.hpp"
#include "../../scene/hittables/hittable_sphere.hpp"

class CornellBoxSphereExampleView : public ExampleView {
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
        drawer->set_static_frame_substeps(1);
        drawer->set_dynamic_frame_substeps(1);
        drawer->set_max_reflections(7);
    }

    void init_scene() override {
        ExampleView::init_scene();

        Material *m_white = new MaterialLambertian({1, 1, 1});
        Material *m_red   = new MaterialLambertian({1, 0, 0});
        Material *m_green = new MaterialLambertian({0, 1, 0});
        Material *m_light = new MaterialLambertianLight({1, 1, 1});
        Material *m_glass = new MaterialDielectric({1, 0.874, 0.768}, 2, -1, 0);

        float lamp_size = 1.6;
        float box_size = 10;

        build_cube(scene->get_root_hittable(), Matrix4f::scale_matrix(box_size, box_size, box_size), (Material*[6]){
                m_white, nullptr,
                m_red, m_green,
                m_white, m_white
        });

        build_rect(scene->get_root_hittable(), m_light, {0, 0, box_size / 2 - FLOAT_EPS}, {lamp_size, lamp_size, 0}, {lamp_size, -lamp_size, 0});

        scene->get_root_hittable()->add_children(new HittableSphere({0, 0, -2}, 2, m_glass));
    }
};