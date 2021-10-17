
#include "build_cube.hpp"
#include "hittables/hittable_list.hpp"
#include "hittables/hittable_triangle.hpp"
#include "../obj/tiny_obj_loader.h"

void build_cube(HittableList* container, const Matrix4f& transform, Material* material, CubeEdges bitmap) {

    Material* array[6] = {
        (bitmap & CubeEdges::x_pos) == CubeEdges::none ? nullptr : material,
        (bitmap & CubeEdges::x_neg) == CubeEdges::none ? nullptr : material,
        (bitmap & CubeEdges::y_pos) == CubeEdges::none ? nullptr : material,
        (bitmap & CubeEdges::y_neg) == CubeEdges::none ? nullptr : material,
        (bitmap & CubeEdges::z_pos) == CubeEdges::none ? nullptr : material,
        (bitmap & CubeEdges::z_neg) == CubeEdges::none ? nullptr : material,
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