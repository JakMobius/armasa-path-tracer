#pragma once

class HittableList;

#include <vector>
#include "hittable.hpp"
#include "../scene_renderer.hpp"

extern const int HittableListType;

class HittableList : public Hittable {
    std::vector<Hittable*> children;

public:
    HittableList(): children() {
        set_gl_buffer_stride(2);
    }

    void add_children(Hittable* hittable) {
        children.push_back(hittable);
        // Two fields + children indices
        set_gl_buffer_stride(2 + children.size());
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
        //auto float_buffer = scene_buffer->get_index_buffer()->get_storage();

        int children_count = (int)children.size();

        index_buffer[index] = HittableListType;
        index_buffer[index + 1] = children_count;

        for(int i = 0; i < children_count; i++) {
            int children_index = renderer->get_hittable_index(children[i]);
            index_buffer[index + 2 + i] = children_index;
        }
    }
};