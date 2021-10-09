
#include "build_cube.hpp"
#include "hittables/hittable_list.hpp"
#include "hittables/hittable_triangle.hpp"
#include "../obj/tiny_obj_loader.h"

namespace CubeEdges {
    const int x_pos = 0b000001;
    const int x_neg = 0b000010;
    const int y_pos = 0b000100;
    const int y_neg = 0b001000;
    const int z_pos = 0b010000;
    const int z_neg = 0b100000;
}

void build_cube(HittableList* container, const Matrix4f& transform, Material* material, int bitmap) {

    Material* array[6] = {
        (bitmap & CubeEdges::x_pos) ? material : nullptr,
        (bitmap & CubeEdges::x_neg) ? material : nullptr,
        (bitmap & CubeEdges::y_pos) ? material : nullptr,
        (bitmap & CubeEdges::y_neg) ? material : nullptr,
        (bitmap & CubeEdges::z_pos) ? material : nullptr,
        (bitmap & CubeEdges::z_neg) ? material : nullptr,
    };

    build_cube(container, transform, array);
}

void build_cube(HittableList* container, const Matrix4f& transform, Material* materials[6]) {

    std::vector<Vec3f> vertices {
            {+0.5, +0.5, -0.5},
            {+0.5, -0.5, -0.5},
            {-0.5, +0.5, -0.5},
            {-0.5, -0.5, -0.5},
            {+0.5, +0.5, +0.5},
            {+0.5, -0.5, +0.5},
            {-0.5, +0.5, +0.5},
            {-0.5, -0.5, +0.5}
    };

    for(auto& vertex : vertices) vertex *= transform;

    if(materials[0]) {
        container->add_children(new HittableTriangle(vertices[0], vertices[5], vertices[1], materials[0]));
        container->add_children(new HittableTriangle(vertices[0], vertices[4], vertices[5], materials[0]));
    }

    if(materials[1]) {
        container->add_children(new HittableTriangle(vertices[2], vertices[7], vertices[6], materials[1]));
        container->add_children(new HittableTriangle(vertices[2], vertices[3], vertices[7], materials[1]));
    }

    if(materials[2]) {
        container->add_children(new HittableTriangle(vertices[0], vertices[2], vertices[6], materials[2]));
        container->add_children(new HittableTriangle(vertices[0], vertices[6], vertices[4], materials[2]));
    }

    if(materials[3]) {
        container->add_children(new HittableTriangle(vertices[1], vertices[7], vertices[3], materials[3]));
        container->add_children(new HittableTriangle(vertices[1], vertices[5], vertices[7], materials[3]));
    }

    if(materials[4]) {
        container->add_children(new HittableTriangle(vertices[4], vertices[7], vertices[5], materials[4]));
        container->add_children(new HittableTriangle(vertices[4], vertices[6], vertices[7], materials[4]));
    }

    if(materials[5]) {
        container->add_children(new HittableTriangle(vertices[0], vertices[1], vertices[3], materials[5]));
        container->add_children(new HittableTriangle(vertices[0], vertices[3], vertices[2], materials[5]));
    }
}