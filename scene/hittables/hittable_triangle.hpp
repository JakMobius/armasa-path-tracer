#pragma once

#include "../../utils/vec3.hpp"
#include "hittable.hpp"
#include "../materials/material.hpp"
#include "../buffer_chunk.hpp"

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
        set_index_buffer_stride(28);

        normal_a = normal_b = normal_c = (point_b - point_a).cross(point_c - point_a).normal();
    }

    void render(SceneRenderer* renderer, BufferChunk* chunk) override {
        int material_index = renderer->get_material_index(material);

        chunk->write_index(HittableTriangleType);
        chunk->write_index(material_index);

        chunk->align();
        chunk->write_vector(point_a);
        chunk->write_vector(point_b - point_a);
        chunk->write_vector(point_c - point_a);
        chunk->write_vector(normal_a);
        chunk->write_vector(normal_b);
        chunk->write_vector(normal_c);
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