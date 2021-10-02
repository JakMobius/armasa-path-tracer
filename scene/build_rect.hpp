#pragma once

#include "../utils/vec3.hpp"
#include "hittables/hittable_list.hpp"
#include "materials/material.hpp"

void build_rect(HittableList* container, Material* material, const Vec3f& center, const Vec3f& first_corner_offset, const Vec3f& second_corner_offset);