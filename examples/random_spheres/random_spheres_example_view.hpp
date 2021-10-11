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

class RandomSpheresExampleView : public ExampleView {
private:
    float random_float(float min = -1, float max = 1) {
        return ((float) rand() / (float) RAND_MAX) * (max - min) + min;
    }

public:

    void init_window() override {
        create_window(1240, 720, 1);
    }

    void init_camera() override {
        ExampleView::init_camera();

        camera->set_position({-5.3, 0, 1});
        camera->set_focus_distance(2);
        camera->matrix = Matrix4f::rotation_y_matrix(-0.4) * camera->matrix;

        controller->set_camera_speed(0.01);
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

        Material *m_grass = new MaterialLambertian({0, 0.8, 0});

        build_rect(scene->get_root_hittable(), m_grass, {0, 0, -0.5}, {100, 100, 0}, {100, -100, 0});

        srand(1);
        create_spheres();
    }

    Material* random_material() {
        int type = rand() % 3;

        Vec3f color = {
            random_float(0, 1),
            random_float(0, 1),
            random_float(0, 1)
        };

        switch(type) {
            case 0: return new MaterialLambertian(color);
            case 1: return new MaterialDielectric(color, random_float(1, 10), random_float(0, 1));
            case 2: return new MaterialMetal(color, random_float(0, 1));
            default: return nullptr;
        }
    }

    void create_spheres() {
        for(int i = 0; i < 400; i++) {
            Vec3f position = {random_float(-4, 4), random_float(-4, 4), 0};
            float radius = random_float(0.1, 0.3);
            auto* sphere = new HittableSphere(position, radius, random_material());
            scene->get_root_hittable()->add_children(sphere);
        }
    }
};