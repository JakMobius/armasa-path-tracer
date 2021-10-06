#pragma once

class HittableList;
class BVHNode;

#include <vector>
#include "hittable.hpp"
#include "../scene_renderer.hpp"

extern const int HittableListType;

class HittableList : public Hittable {
    std::vector<Hittable*> children;

public:
    HittableList(): children() {
        set_gl_buffer_stride(8);
    }

    void add_children(Hittable* hittable) {
        children.push_back(hittable);
        // Two fields + children indices
        set_gl_buffer_stride(std::max(8, 2 + (int)children.size()));
    };

    void register_hittables(SceneRenderer* renderer) override {
        for(auto child : children) renderer->enqueue_hittable_render(child);
    }

    void register_materials(SceneRenderer* renderer) override {
        for(auto child : children) child->register_materials(renderer);
    }

    void render(SceneRenderer* renderer, int index) override {

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();
        auto& float_buffer = scene_buffer->get_float_buffer()->get_storage();

        int children_count = (int)children.size();

        index_buffer[index] = HittableListType;
        index_buffer[index + 1] = children_count;

        AABB aabb = get_bounding_box();

        float_buffer[index]     = aabb.lower[0];
        float_buffer[index + 1] = aabb.lower[1];
        float_buffer[index + 2] = aabb.lower[2];
        float_buffer[index + 4] = aabb.upper[0];
        float_buffer[index + 5] = aabb.upper[1];
        float_buffer[index + 6] = aabb.upper[2];

        for(int i = 0; i < children_count; i++) {
            int children_index = renderer->get_hittable_index(children[i]);
            index_buffer[index + 2 + i] = children_index;
        }
    }

    Hittable* to_bvh_node() override;

    void update_aabb() override {
        if(children.empty()) {
            bounding_box.lower = {0, 0, 0};
            bounding_box.upper = {0, 0, 0};
        } else {
            bounding_box = children[0]->get_bounding_box();
            for(int i = 1; i < (int)children.size(); i++) {
                bounding_box.extend(children[i]->get_bounding_box());
            }
        }
    }

    std::vector<Hittable*>& get_children() { return children; }

    AABB get_bounding_box_indices(int from, int to) {
        if(children.empty()) {
            return {};
        }

        AABB result = children[from]->get_bounding_box();
        for(int i = from + 1; i < to; i++) {
            result.extend(children[i]->get_bounding_box());
        }
        return result;
    }
};