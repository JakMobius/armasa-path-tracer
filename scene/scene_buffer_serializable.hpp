#pragma once

#include "scene_renderer.hpp"

class SceneBufferSerializable;
class Scene;

class SceneBufferSerializable {
    int gl_buffer_index = 0;
    int gl_buffer_stride = 0;
    Scene* scene = nullptr;

public:

    void set_gl_buffer_index(int p_gl_buffer_index) { gl_buffer_index = p_gl_buffer_index; };
    int get_gl_buffer_index() { return gl_buffer_index; };
    void set_gl_buffer_stride(int stride);
    int get_gl_buffer_stride() { return gl_buffer_stride; }

    virtual void render(SceneRenderer* renderer, int index) {};

    Scene* get_scene() { return scene; }
    void set_scene(Scene* p_scene) { scene = p_scene; }
};