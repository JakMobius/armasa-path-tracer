//
// Created by Артем on 01.10.2021.
//

#include "hittable_list.hpp"
#include "bvh_node.hpp"

const int HittableListType = 0;

Hittable* HittableList::to_bvh_node() {
    return new BVHNode(this, 0, children.size());
}
