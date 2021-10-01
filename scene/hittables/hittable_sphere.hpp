#pragma once

#include "hittable.hpp"
#include "../../utils/vec3.hpp"

const int HittableSphereType = 1;

class HittableSphere : public Hittable {
    Vec3f position;
    float radius;
public:

    HittableSphere(const Vec3f& position, float radius): Hittable(), position(position), radius(radius) {
        set_gl_buffer_stride(5);
    }

    void render(int* index_buffer, float* float_buffer) override {
        int index = get_gl_buffer_index();

        index_buffer[index] = HittableSphereType;
        float_buffer[index + 1] = position[0];
        float_buffer[index + 2] = position[1];
        float_buffer[index + 3] = position[2];
        float_buffer[index + 4] = radius;
    }

    void set_position(const Vec3f& p_position) { position = p_position; }
    const Vec3f& get_position() const { return position; }
    void set_radius(float p_radius) { radius = p_radius; }
    float get_radius() const { return radius; }
};