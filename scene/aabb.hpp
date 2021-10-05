#pragma once

#include "../utils/vec3.hpp"

struct AABB {
    Vec3f lower;
    Vec3f upper;

    AABB(const Vec3f& lower, const Vec3f& upper): lower(lower), upper(upper) {}
    AABB(): lower({0, 0, 0}), upper({0, 0, 0}) {}

    void extend(const AABB& other) {
        for(int i = 0; i < 3; i++) {
            if(lower[i] > other.lower[i]) lower.set(i, other.lower[i]);
            if(upper[i] < other.upper[i]) upper.set(i, other.upper[i]);
        }
    }

    double get_effective_size() {
        return fabs(upper[0] - lower[0]) + fabs(upper[1] - lower[1]) + fabs(upper[2] - lower[2]);
    }
};