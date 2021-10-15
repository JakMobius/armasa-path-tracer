#pragma once

struct BVHNode;

#include "../hittable_list.hpp"

extern bool (*BVH_AABB_COMPARATORS[3])(Hittable *a, Hittable *b);

namespace BVHNodeFlags {
    enum BVHNodeFlags : unsigned int {
        is_leaf    = 1u << 28u,
        x_positive = 1u << 29u,
        y_positive = 1u << 30u,
        z_positive = 1u << 31u
    };

    BVHNodeFlags n_positive(unsigned int n);
}

struct BVHNode {

    BVHNode(): flags(0), leaf(nullptr) {};
    ~BVHNode() = default;

    double get_effective_split_size(std::vector<Hittable*>* list, int from, int to, int axis);

    unsigned int flags;
    Hittable* leaf;
    AABB bounding_box {};

    Vec3f get_masked_aabb_vector();
    Vec3f get_opposite_masked_aabb_vector();
};