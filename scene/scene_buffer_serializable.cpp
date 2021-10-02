//
// Created by Артем on 02.10.2021.
//

#include "scene_buffer_serializable.hpp"
#include "scene.hpp"

void SceneBufferSerializable::set_gl_buffer_stride(int stride) {
    gl_buffer_stride = stride;
    if(scene) scene->invalidate_buffer_layout();
}