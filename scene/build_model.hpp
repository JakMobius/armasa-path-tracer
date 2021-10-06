#pragma once

#include "../obj/tiny_obj_loader.h"
#include "../scene/hittables/hittable_list.hpp"

bool build_model(HittableList* container, const char* path, const Matrix4f& transform, Material* material);