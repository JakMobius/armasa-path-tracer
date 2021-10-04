#pragma once

#include "hittable.hpp"
#include "../../utils/vec3.hpp"
#include "../materials/material.hpp"

const int HittableSphereType = 1;

class HittableSphere : public Hittable {
    Vec3f position;
    float radius;
    Material* material;
public:

    HittableSphere(const Vec3f& position, float radius, Material* material): Hittable(), position(position), radius(radius), material(material) {
        set_gl_buffer_stride(4);
    }

    void render(SceneRenderer* renderer, int index) override {
        int material_index = renderer->get_material_index(material);

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();
        auto& float_buffer = scene_buffer->get_float_buffer()->get_storage();

        index_buffer[index] = HittableSphereType;
        index_buffer[index + 1] = material_index;

        float_buffer[index + 0] = position[0];
        float_buffer[index + 1] = position[1];
        float_buffer[index + 2] = position[2];
        float_buffer[index + 3] = radius;
    }

    void register_materials(SceneRenderer* renderer) override {
        renderer->register_material(material);
    }

    void set_position(const Vec3f& p_position) { position = p_position; }
    const Vec3f& get_position() const { return position; }
    void set_radius(float p_radius) { radius = p_radius; }
    float get_radius() const { return radius; }

    void update_aabb() override {
        Vec3f radius_vector { radius, radius, radius };

        bounding_box.lower = position;
        bounding_box.upper = position;

        bounding_box.lower -= radius_vector;
        bounding_box.upper += radius_vector;
    }
};