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

static bool (*aabb_comparators[3])(Hittable *a, Hittable *b) = { &aabb_comparator_x, &aabb_comparator_y, &aabb_comparator_z };

int BVHNode::maximum_nodes = 4;

double BVHNode::get_effective_split_size(HittableList* list, int from, int to, int axis) {
    auto comparator = aabb_comparators[axis];

    int count = to - from;

    std::sort(list->get_children().begin() + from, list->get_children().begin() + to, comparator);
    int split_index = from + count / 2;

    AABB left = list->get_bounding_box_indices(from, split_index);
    AABB right = list->get_bounding_box_indices(split_index, to);

    return left.get_effective_size() + right.get_effective_size();
}

void BVHNode::generate_bvh(HittableList* list, int from, int to) {
    int count = to - from;

    if(count <= BVHNode::maximum_nodes) {
        for(int i = from; i < to; i++) {
            add_children(list->get_children()[i]->to_bvh_node());
        }
        return;
    }

    double est_x = get_effective_split_size(list, from, to, 0);
    double est_y = get_effective_split_size(list, from, to, 1);
    double est_z = get_effective_split_size(list, from, to, 2);

    int split_axis = 0;

    if (est_x <= est_y && est_x <= est_z) split_axis = 0;
    else if (est_y <= est_x && est_y <= est_z) split_axis = 1;
    else split_axis = 2;

    auto comparator = aabb_comparators[split_axis];

    std::sort(list->get_children().begin() + from, list->get_children().begin() + to, comparator);

    int split_index = from + count / 2;

    add_children(new BVHNode(list, from, split_index));
    add_children(new BVHNode(list, split_index, to));
}
