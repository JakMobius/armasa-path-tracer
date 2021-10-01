#pragma once

class Scene;
class HittableList;

#include "hittables/hittable.hpp"
#include "../graphics/gl/gl_texture_buffer.hpp"

class Scene {
    HittableList* root_hittable;
    int data_length = -1;

    void layout_buffer();

public:
    Scene();
    ~Scene();

    void invalidate_buffer_layout() { data_length = -1; }

    void render(Graphics::GLTextureBuffer<int>* index_buffer, Graphics::GLTextureBuffer<float>* float_buffer);

    HittableList* get_root_hittable() const { return root_hittable; }
};