#pragma once

class Hittable;

#include <queue>
#include "../scene.hpp"
#include "../scene_buffer_serializable.hpp"
#include "../scene_renderer.hpp"

class Hittable : public SceneBufferSerializable {

public:
    Hittable() {};
    virtual ~Hittable() {};

    virtual void register_hittables(SceneRenderer* renderer) {};
    virtual void register_materials(SceneRenderer* renderer) {};
};