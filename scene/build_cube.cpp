
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
        container->add_children(new HittableTriangle(vertices[0], vertices[1], vertices[5], materials[0]));
        container->add_children(new HittableTriangle(vertices[0], vertices[4], vertices[5], materials[0]));
    }

    if(materials[1]) {
        container->add_children(new HittableTriangle(vertices[2], vertices[6], vertices[7], materials[1]));
        container->add_children(new HittableTriangle(vertices[2], vertices[3], vertices[7], materials[1]));
    }

    if(materials[2]) {
        container->add_children(new HittableTriangle(vertices[0], vertices[2], vertices[6], materials[2]));
        container->add_children(new HittableTriangle(vertices[0], vertices[4], vertices[6], materials[2]));
    }

    if(materials[3]) {
        container->add_children(new HittableTriangle(vertices[1], vertices[3], vertices[7], materials[3]));
        container->add_children(new HittableTriangle(vertices[1], vertices[5], vertices[7], materials[3]));
    }

    if(materials[4]) {
        container->add_children(new HittableTriangle(vertices[4], vertices[5], vertices[7], materials[4]));
        container->add_children(new HittableTriangle(vertices[4], vertices[6], vertices[7], materials[4]));
    }

    if(materials[5]) {
        container->add_children(new HittableTriangle(vertices[0], vertices[1], vertices[3], materials[5]));
        container->add_children(new HittableTriangle(vertices[0], vertices[2], vertices[3], materials[5]));
    }
}

bool build_model(HittableList* container, const char* path, const Matrix4f& transform, Material* material) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::string err;

    if(!tinyobj::LoadObj(&attrib, &shapes, nullptr, &err, path, nullptr)) {
        return false;
    }

    for(auto& shape : shapes) {
        for(int i = 0; i < (int)shape.mesh.indices.size(); i += 3) {
            int index_a = shape.mesh.indices[i].vertex_index;
            int index_b = shape.mesh.indices[i + 1].vertex_index;
            int index_c = shape.mesh.indices[i + 2].vertex_index;

            Vec3f point_a { attrib.vertices[index_a * 3 + 0], attrib.vertices[index_a * 3 + 1], attrib.vertices[index_a * 3 + 2] };
            Vec3f point_b { attrib.vertices[index_b * 3 + 0], attrib.vertices[index_b * 3 + 1], attrib.vertices[index_b * 3 + 2] };
            Vec3f point_c { attrib.vertices[index_c * 3 + 0], attrib.vertices[index_c * 3 + 1], attrib.vertices[index_c * 3 + 2] };

            point_a *= transform;
            point_b *= transform;
            point_c *= transform;

            index_a = shape.mesh.indices[i].normal_index;
            index_b = shape.mesh.indices[i + 1].normal_index;
            index_c = shape.mesh.indices[i + 2].normal_index;

            Vec3f normal_a { attrib.normals[index_a * 3 + 0], attrib.normals[index_a * 3 + 1], attrib.normals[index_a * 3 + 2] };
            Vec3f normal_b { attrib.normals[index_b * 3 + 0], attrib.normals[index_b * 3 + 1], attrib.normals[index_b * 3 + 2] };
            Vec3f normal_c { attrib.normals[index_c * 3 + 0], attrib.normals[index_c * 3 + 1], attrib.normals[index_c * 3 + 2] };

            normal_a.transform_unbound(transform);
            normal_b.transform_unbound(transform);
            normal_c.transform_unbound(transform);

            auto* triangle = new HittableTriangle(point_a, point_b, point_c, material);
            triangle->set_normals(normal_a.normal(), normal_b.normal(), normal_c.normal());

            container->add_children(triangle);
        }
    }

    return true;
}