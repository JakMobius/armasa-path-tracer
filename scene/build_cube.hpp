#pragma once

#include "build_cube.hpp"
#include "../utils/vec3.hpp"
#include "scene.hpp"
#include <string>

namespace CubeEdges {
    extern const int x_pos;
    extern const int x_neg;
    extern const int y_pos;
    extern const int y_neg;
    extern const int z_pos;
    extern const int z_neg;
}

void build_cube(HittableList* container, const Matrix4f& transform, Material* material, int bitmap = ~0);
void build_cube(HittableList* container, const Matrix4f& transform, Material* materials[6]);
bool build_model(HittableList* container, const char* path, const Matrix4f& transform, Material* material);