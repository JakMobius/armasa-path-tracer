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
        create_window(500, 500, 2);
    }

    void init_camera() override {
        ExampleView::init_camera();
    }

    void init_drawer() override {
        ExampleView::init_drawer();

    }

    void init_scene() override {
        ExampleView::init_scene();

    }
};