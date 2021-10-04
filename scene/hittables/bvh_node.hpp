#pragma once

#include "hittable_list.hpp"

class BVHNode : public HittableList {

    static int maximum_nodes;

    double get_effective_split_size(HittableList* list, int from, int to, int axis);
    void generate_bvh(HittableList* list, int from, int to);

public:
    BVHNode(HittableList* list, int from, int to): HittableList() {
        generate_bvh(list, from, to);
    }

    virtual void destroy_bvh() override {
        for(auto child : get_children()) {
            child->destroy_bvh();
        }
    }
};