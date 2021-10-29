//#pragma once
//
//#include "../../scene/scene.hpp"
//#include "../../scene/materials/material_lambertian.hpp"
//#include "../../scene/materials/material_lambertian_light.hpp"
//#include "../example_view.hpp"
//#include "../../scene/build_cube.hpp"
//#include "../../scene/build_rect.hpp"
//#include "../../scene/materials/material_metal.hpp"
//#include "../../scene/materials/material_dielectric.hpp"
//#include "../../scene/hittables/hittable_sphere.hpp"
//#include "../../scene/build_model.hpp"
//#include "../../scene/hittables/hittable_triangle.hpp"
//#include "../../utils/vec2.hpp"
//
//class PlaygroundView : public ExampleView {
//public:
//
//    void init_window() override {
////        create_window(3072, 1920, 0.5);
//        create_window(500, 500, 2);
//    }
//
//    void init_camera() override {
//        ExampleView::init_camera();
//
//        camera->set_position({-8, -0.5, 0});
//        camera->set_focus_distance(2.5);
//        camera->matrix = Matrix4f::rotation_y_matrix(-0.2) * camera->matrix;
//        controller->set_camera_speed(0.1);
//    }
//
//    void init_drawer() override {
//        ExampleView::init_drawer();
//
//        drawer->get_post_processing_program()->set_brightness(1.0);
//        drawer->get_post_processing_program()->set_gamma(1);
//        drawer->set_static_frame_substeps(20);
//        drawer->set_dynamic_frame_substeps(1);
//        drawer->set_max_reflections(20);
//        drawer->get_tracer_program()->set_background_color({0.05, 0.05, 0.1});
//
//        frames_logging = 1;
//    }
//
//    void init_scene() override {
//        ExampleView::init_scene();
//
//    }
//};

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

class PlaygroundView: public ExampleView {
public:
    PlaygroundView(): ExampleView() {}

    void init_window() override {
        create_window(300, 300, 1);
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
        drawer->set_static_frame_substeps(1);
        drawer->set_dynamic_frame_substeps(1);
        drawer->set_max_reflections(7);

        frames_logging = 1;
    }

    void init_scene() override {
        ExampleView::init_scene();
    }
};