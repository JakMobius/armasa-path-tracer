//
// Created by Артем on 02.10.2021.
//

#include "build_rect.hpp"
#include "hittables/hittable_list.hpp"
#include "hittables/hittable_triangle.hpp"

void build_rect(HittableList* container, Material* material, const Vec3f& center, const Vec3f& first_corner_offset, const Vec3f& second_corner_offset) {
    container->add_children(new HittableTriangle(center + first_corner_offset, center + second_corner_offset, center - first_corner_offset, material));
    container->add_children(new HittableTriangle(center + first_corner_offset, center - first_corner_offset, center - second_corner_offset, material));
}