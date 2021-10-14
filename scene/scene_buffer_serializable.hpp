#pragma once

#include "scene_renderer.hpp"
#include "buffer_chunk.hpp"

class SceneBufferSerializable;
class Scene;

class SceneBufferSerializable {

    int gl_index_buffer_stride = 0;
    Scene* scene = nullptr;

public:

    void set_index_buffer_stride(int stride);
    int get_index_buffer_stride() const { return gl_index_buffer_stride; }

    virtual void render(SceneRenderer* renderer, BufferChunk* chunk);

    Scene* get_scene() { return scene; }
    void set_scene(Scene* p_scene) { scene = p_scene; }
};