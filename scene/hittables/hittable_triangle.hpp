#pragma once

#include "../../utils/vec3.hpp"
#include "hittable.hpp"
#include "../materials/material.hpp"

extern const int HittableTriangleType;

class HittableTriangle : public Hittable {
    Vec3f point_a;
    Vec3f point_b;
    Vec3f point_c;
    Material* material;

public:

    HittableTriangle(const Vec3f& point_a, const Vec3f& point_b, const Vec3f& point_c, Material* material):
        Hittable(), point_a(point_a), point_b(point_b), point_c(point_c), material(material) {
        set_gl_buffer_stride(12);
    }

    void render(SceneRenderer* renderer, int index) override {
        int material_index = renderer->get_material_index(material);

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();
        auto& float_buffer = scene_buffer->get_float_buffer()->get_storage();

        index_buffer[index]     = HittableTriangleType;
        index_buffer[index + 1] = material_index;
        float_buffer[index + 0] = point_a[0];
        float_buffer[index + 1] = point_a[1];
        float_buffer[index + 2] = point_a[2];
        float_buffer[index + 4] = point_b[0];
        float_buffer[index + 5] = point_b[1];
        float_buffer[index + 6] = point_b[2];
        float_buffer[index + 8] = point_c[0];
        float_buffer[index + 9] = point_c[1];
        float_buffer[index + 10] = point_c[2];
    }

    void register_materials(SceneRenderer* renderer) override {
        renderer->register_material(material);
    }

    void update_aabb() override {
        for(int i = 0; i < 3; i++) {
            bounding_box.lower.set(i, std::min(point_a[i], std::min(point_b[i], point_c[i])) - Vec3f::epsilon);
            bounding_box.upper.set(i, std::max(point_a[i], std::max(point_b[i], point_c[i])) + Vec3f::epsilon);
        }
    }

    void set_point_a(const Vec3f& p_point_a) { point_a = p_point_a; }
    void set_point_b(const Vec3f& p_point_b) { point_b = p_point_b; }
    void set_point_c(const Vec3f& p_point_c) { point_c = p_point_c; }

    const Vec3f& get_point_a() { return point_a; }
    const Vec3f& get_point_b() { return point_b; }
    const Vec3f& get_point_c() { return point_c; }
};