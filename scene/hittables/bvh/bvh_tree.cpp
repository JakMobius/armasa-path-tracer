//
// Created by Артем on 13.10.2021.
//

#include "bvh_tree.hpp"

void BVHTree::generate_bvh(int node_index, int from, int to, std::vector<Hittable*>* list) {
    int count = to - from;

    BVHNode* node = get_node(node_index);

    if(count == 0) return;
    if(count == 1) {
        node->leaf = (*list)[from];
        node->flags |= BVHNodeFlags::is_leaf;
        node->bounding_box = node->leaf->get_bounding_box();
        return;
    }

    double est_x = node->get_effective_split_size(list, from, to, 0);
    double est_y = node->get_effective_split_size(list, from, to, 1);
    double est_z = node->get_effective_split_size(list, from, to, 2);

    int split_axis = 0;

    if (est_x <= est_y && est_x <= est_z) split_axis = 0;
    else if (est_y <= est_x && est_y <= est_z) split_axis = 1;
    else split_axis = 2;

    auto comparator = BVH_AABB_COMPARATORS[split_axis];

    std::sort(list->begin() + from, list->begin() + to, comparator);

    int split_index = from + count / 2;

    generate_bvh(node_index * 2, from, split_index, list);
    generate_bvh(node_index * 2 + 1, split_index, to, list);

    // In case internal storage was reallocated
    node = get_node(node_index);

    BVHNode* left = get_node(node_index * 2);
    BVHNode* right = get_node(node_index * 2 + 1);

    node->bounding_box = left->bounding_box;
    node->bounding_box.extend(right->bounding_box);

    for(int i = 0; i < 3; i++) {
        if(node->bounding_box.upper[i] == left->bounding_box.upper[i]) left->flags |= BVHNodeFlags::n_positive(i);
        if(node->bounding_box.upper[i] == right->bounding_box.upper[i]) right->flags |= BVHNodeFlags::n_positive(i);
    }
}

void BVHTree::update_aabb() {
    bounding_box = nodes[0].bounding_box;
}
