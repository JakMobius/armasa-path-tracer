#pragma once

#include "../graphics/gl/gl_texture_buffer.hpp"

class SceneBuffer {
    Graphics::GLTextureBuffer<float>* scene_float_buffer;
    Graphics::GLTextureBuffer<int>* scene_index_buffer;
    bool needs_synchronization = true;
    int entry_hittable_index = 0;

public:
    SceneBuffer() {
        scene_float_buffer = new Graphics::GLTextureBuffer<float>(
            Graphics::GLTextureInternalFormat::rgba32f,
            Graphics::GLBufferUsage::dynamic_draw
        );
        scene_index_buffer = new Graphics::GLTextureBuffer<int>(
            Graphics::GLTextureInternalFormat::rgba32i,
            Graphics::GLBufferUsage::dynamic_draw
        );
    }

    void create_buffers() {
        scene_float_buffer->create_buffer();
        scene_index_buffer->create_buffer();
    }

    void bind_buffers(GLenum index_buffer_index, GLenum float_buffer_index) {
        scene_index_buffer->bind_texture(index_buffer_index);
        scene_float_buffer->bind_texture(float_buffer_index);
    }

    void synchronize_if_needed() {
        if(!needs_synchronization) return;
        needs_synchronization = false;

        scene_float_buffer->synchronize();
        scene_index_buffer->synchronize();
    }

    void set_needs_synchronize() { needs_synchronization = true; }
    int get_entry_hittable_index() { return entry_hittable_index; }
    void set_entry_hittable_index(int p_entry_hittable_index) { entry_hittable_index = p_entry_hittable_index; }

    Graphics::GLTextureBuffer<int>* get_index_buffer() { return scene_index_buffer; }
    Graphics::GLTextureBuffer<float>* get_float_buffer() { return scene_float_buffer; }

    void require_capacity(int capacity) {
        if((int)scene_index_buffer->get_storage().size() < capacity) {
            scene_index_buffer->get_storage().resize(capacity);
        }

        if((int)scene_float_buffer->get_storage().size() < capacity) {
            scene_float_buffer->get_storage().resize(capacity);
        }
    }
};