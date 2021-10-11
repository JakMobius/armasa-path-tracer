//
// Created by Артем on 02.10.2021.
//

#include "scene_buffer_serializable.hpp"
#include "scene.hpp"
#include "buffer_chunk.hpp"

void SceneBufferSerializable::set_index_buffer_stride(int stride) {
    gl_index_buffer_stride = stride;
}

void SceneBufferSerializable::set_float_buffer_stride(int stride) {
    gl_float_buffer_stride = stride;
}

void SceneBufferSerializable::render(SceneRenderer* renderer, BufferChunk* chunk) {}
