#pragma once

#include "../../utils/vec3.hpp"
#include "hittable.hpp"
#include "../materials/material.hpp"

extern const int HittableTriangleType;

class HittableTriangle : public Hittable {
    Vec3f point_a;
    Vec3f point_b;
    Vec3f point_c;

    Vec3f normal_a;
    Vec3f normal_b;
    Vec3f normal_c;

    Material* material;

public:

    HittableTriangle(const Vec3f& point_a, const Vec3f& point_b, const Vec3f& point_c, Material* material):
        Hittable(), point_a(point_a), point_b(point_b), point_c(point_c), material(material) {
        set_gl_buffer_stride(24);

        normal_a = normal_b = normal_c = (point_b - point_a).cross(point_c - point_a).normal();
    }

    void render(SceneRenderer* renderer, int index) override {
        int material_index = renderer->get_material_index(material);

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();

        index_buffer[index]     = HittableTriangleType;
        index_buffer[index + 1] = material_index;
        scene_buffer->write_vector(point_a, index + 0);
        scene_buffer->write_vector(point_b - point_a, index + 4);
        scene_buffer->write_vector(point_c - point_a, index + 8);
        scene_buffer->write_vector(normal_a, index + 12);
        scene_buffer->write_vector(normal_b, index + 16);
        scene_buffer->write_vector(normal_c, index + 20);
    }

    void set_normals(const Vec3f& p_normal_a, const Vec3f& p_normal_b, const Vec3f& p_normal_c) {
        normal_a = p_normal_a;
        normal_b = p_normal_b;
        normal_c = p_normal_c;
    }

    void register_materials(SceneRenderer* renderer) override {
        renderer->register_material(material);
    }

    void update_aabb() override {
        bounding_box.lower = point_a;
        bounding_box.upper = point_a;

        for(int i = 0; i < 3; i++) {
            bounding_box.lower.set(i, std::min(point_a[i], std::min(point_b[i], point_c[i])));
            bounding_box.upper.set(i, std::max(point_a[i], std::max(point_b[i], point_c[i])));
        }

        Vec3f size = bounding_box.upper - bounding_box.lower;
        Vec3f new_size = size;

        for(int i = 0; i < 3; i++) {
            if(new_size[i] < FLOAT_EPS) new_size.set(i, FLOAT_EPS);
        }

        Vec3f offset = (new_size - size) / 2;
        bounding_box.lower -= offset;
        bounding_box.upper += offset;
    }

    void set_point_a(const Vec3f& p_point_a) { point_a = p_point_a; }
    void set_point_b(const Vec3f& p_point_b) { point_b = p_point_b; }
    void set_point_c(const Vec3f& p_point_c) { point_c = p_point_c; }

    const Vec3f& get_point_a() { return point_a; }
    const Vec3f& get_point_b() { return point_b; }
    const Vec3f& get_point_c() { return point_c; }
};