#pragma once

class Scene;
class HittableList;

#include "hittables/hittable.hpp"
#include "../graphics/gl/gl_texture_buffer.hpp"
#include "scene_buffer.hpp"
#include "scene_renderer.hpp"

class Scene {
    HittableList* root_hittable;
    SceneRenderer renderer;

public:
    Scene();
    ~Scene();

    void invalidate_buffer_layout() { renderer.invalidate_layout(); }

    HittableList* get_root_hittable() const { return root_hittable; }

    SceneRenderer* get_renderer() { return &renderer; }
};