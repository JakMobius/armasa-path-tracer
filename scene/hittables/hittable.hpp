#pragma once

class Hittable;

#include <queue>
#include "../scene.hpp"

class Hittable {
    int gl_buffer_index = 0;
    int gl_buffer_stride = 0;
    Scene* scene = nullptr;

public:
    Hittable() {};
    virtual ~Hittable() {};

    virtual void update_data(int* int_data, float* float_data) {};

    virtual void layout(std::queue<Hittable*>& queue) {};
    virtual void render(int* index_buffer, float* float_buffer) {};

    Scene* get_scene() { return scene; }
    void set_scene(Scene* p_scene) { scene = p_scene; }
    void set_gl_buffer_index(int p_gl_buffer_index) { gl_buffer_index = p_gl_buffer_index; };
    int get_gl_buffer_index() { return gl_buffer_index; };
    void set_gl_buffer_stride(int stride);
    int get_gl_buffer_stride() { return gl_buffer_stride; }
};