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

class PlaygroundView : public ExampleView {
public:

    void init_window() override {
        create_window(1240, 720, 1);
    }

    void init_camera() override {
        ExampleView::init_camera();

        camera->set_position({-5.3, 0, 1});
        camera->set_focus_distance(2);
        camera->matrix = Matrix4f::rotation_y_matrix(-0.4) * camera->matrix;

//        controller->set_camera_speed(0.01);
    }

    void init_drawer() override {
        ExampleView::init_drawer();

        drawer->get_post_processing_program()->set_brightness(1.0);
        drawer->get_post_processing_program()->set_gamma(1.0);
        drawer->set_static_frame_substeps(4);
        drawer->set_dynamic_frame_substeps(1);
        drawer->set_max_reflections(7);
        drawer->get_tracer_program()->set_background_color({0.8, 0.8, 1.0});

//        switch_to_dynamic_mode = false;
    }

    void init_scene() override {
        ExampleView::init_scene();

        Material *m_light = new MaterialLambertianLight({0, 1, 0});

        scene->get_root_hittable()->add_children(new HittableSphere({0, 0, 0}, 0.8, m_light));
        scene->get_root_hittable()->add_children(new HittableSphere({0, 2, 0}, 0.8, m_light));
        scene->get_root_hittable()->add_children(new HittableSphere({2, 2, 0}, 0.8, m_light));
        scene->get_root_hittable()->add_children(new HittableSphere({2, 0, 2}, 0.8, m_light));
    }
};