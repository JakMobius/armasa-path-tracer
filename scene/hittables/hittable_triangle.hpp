#pragma once

#include "../../utils/vec3.hpp"
#include "hittable.hpp"

const int HittableTriangleType = 2;

class HittableTriangle : public Hittable {
    Vec3f point_a;
    Vec3f point_b;
    Vec3f point_c;

public:

    HittableTriangle(const Vec3f& point_a, const Vec3f& point_b, const Vec3f& point_c):
        Hittable(), point_a(point_a), point_b(point_b), point_c(point_c) {
        set_gl_buffer_stride(10);
    }

    void render(int* index_buffer, float* float_buffer) override {
        int index = get_gl_buffer_index();

        index_buffer[index] = HittableTriangleType;
        float_buffer[index + 1] = point_a[0];
        float_buffer[index + 2] = point_a[1];
        float_buffer[index + 3] = point_a[2];
        float_buffer[index + 4] = point_b[0];
        float_buffer[index + 5] = point_b[1];
        float_buffer[index + 6] = point_b[2];
        float_buffer[index + 7] = point_c[0];
        float_buffer[index + 8] = point_c[1];
        float_buffer[index + 9] = point_c[2];
    }

    void set_point_a(const Vec3f& p_point_a) { point_a = p_point_a; }
    void set_point_b(const Vec3f& p_point_b) { point_b = p_point_b; }
    void set_point_c(const Vec3f& p_point_c) { point_c = p_point_c; }

    const Vec3f& get_point_a() { return point_a; }
    const Vec3f& get_point_b() { return point_b; }
    const Vec3f& get_point_c() { return point_c; }
};