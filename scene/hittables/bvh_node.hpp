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

    virtual bool is_bvh_node() override { return true; };

    void dump(int depth = 1) {
        AABB box = get_bounding_box();
        std::cout << "Node: (" << box.lower[0] << "," << box.lower[1] << "," << box.lower[2] << "), (" << box.upper[0] << "," << box.upper[1] << "," << box.upper[2] << ")\n";
        for(int i = 0; i < get_children().size(); i++) {
            auto child = get_children()[i];
            for(int j = 0; j < depth; j++) std::cout << "  ";
            std::cout << " [" << i << "] = ";
            if(child->is_bvh_node()) {
                ((BVHNode*) child)->dump(depth + 1);
            }
            else std::cout << "<leaf>\n";
        }
    }
};