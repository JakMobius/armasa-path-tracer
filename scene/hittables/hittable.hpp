#pragma once

class Hittable;

#include <queue>
#include "../scene.hpp"
#include "../scene_buffer_serializable.hpp"
#include "../scene_renderer.hpp"
#include "../aabb.hpp"

class Hittable : public SceneBufferSerializable {
protected:
    AABB bounding_box;
    bool bounding_box_valid = false;

public:
    Hittable(): bounding_box() {};
    Hittable(Hittable&& move) = delete;
    virtual ~Hittable() {};

    virtual void register_hittables(SceneRenderer* renderer);
    virtual void register_materials(SceneRenderer* renderer);
    virtual void flatten(std::vector<Hittable*>* storage) { storage->push_back(this); };
    virtual void update_aabb() = 0;

    AABB get_bounding_box() {
        if(!bounding_box_valid) {
            update_aabb();
            bounding_box_valid = true;
        }
        return bounding_box;
    }
};