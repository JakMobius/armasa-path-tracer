//
// Created by Артем on 01.10.2021.
//

#include "hittable.hpp"

void Hittable::set_gl_buffer_stride(int stride) {
    gl_buffer_stride = stride;
    if(scene) scene->invalidate_buffer_layout();
}

