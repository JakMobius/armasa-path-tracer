//
// Created by Артем on 04.10.2021.
//

#include "bvh_node.hpp"

static inline bool aabb_comparator(Hittable *a, Hittable *b, int axis) {
    AABB box_a = a->get_bounding_box();
    AABB box_b = b->get_bounding_box();

    return box_a.lower[axis] < box_b.lower[axis];
}

static bool aabb_comparator_x(Hittable *a, Hittable *b) { return aabb_comparator(a, b, 0); }
static bool aabb_comparator_y(Hittable *a, Hittable *b) { return aabb_comparator(a, b, 1); }
static bool aabb_comparator_z(Hittable *a, Hittable *b) { return aabb_comparator(a, b, 2); }

bool (*BVH_AABB_COMPARATORS[3])(Hittable *a, Hittable *b) = {&aabb_comparator_x, &aabb_comparator_y, &aabb_comparator_z };

double BVHNode::get_effective_split_size(std::vector<Hittable*>* list, int from, int to, int axis) {
    auto comparator = BVH_AABB_COMPARATORS[axis];

    int count = to - from;

    std::sort(list->begin() + from, list->begin() + to, comparator);
    int split_index = from + count / 2;

    AABB left = (*list)[from]->get_bounding_box();
    AABB right = (*list)[split_index]->get_bounding_box();

    for(int i = from; i < split_index; i++) left.extend((*list)[i]->get_bounding_box());
    for(int i = split_index; i < to; i++) left.extend((*list)[i]->get_bounding_box());

    return left.get_effective_size() + right.get_effective_size();
}

Vec3f BVHNode::get_masked_aabb_vector() {
    return {
        (flags & BVHNodeFlags::x_positive) ? bounding_box.lower[0] : bounding_box.upper[0],
        (flags & BVHNodeFlags::y_positive) ? bounding_box.lower[1] : bounding_box.upper[1],
        (flags & BVHNodeFlags::z_positive) ? bounding_box.lower[2] : bounding_box.upper[2]
    };
}

BVHNodeFlags::BVHNodeFlags BVHNodeFlags::n_positive(unsigned int n) {
    return (BVHNodeFlags)(1u << (n + 29u));
}
