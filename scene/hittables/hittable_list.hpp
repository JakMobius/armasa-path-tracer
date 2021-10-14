#pragma once

class HittableList;
struct BVHNode;

#include <vector>
#include "hittable.hpp"
#include "../scene_renderer.hpp"
#include "../buffer_chunk.hpp"

extern const int HittableListType;

class HittableList : public Hittable {
    std::vector<Hittable*> children;

public:
    HittableList(): children() {
        set_index_buffer_stride(12);
    }

    void add_children(Hittable* hittable) {
        children.push_back(hittable);
        // Two fields + children indices
        set_index_buffer_stride(12 + children.size());
    };

    void register_hittables(SceneRenderer* renderer) override {
        for(auto child : children) renderer->enqueue_hittable_render(child);
    }

    void register_materials(SceneRenderer* renderer) override {
        for(auto child : children) child->register_materials(renderer);
    }

    void render(SceneRenderer* renderer, BufferChunk* chunk) override {
        int children_count = (int)children.size();

        chunk->write_index(HittableListType | (children_count << 3));

        for(int i = 0; i < children_count; i++) {
            int children_index = renderer->get_hittable_index(children[i]);
            chunk->write_index(children_index);
        }

        AABB aabb = get_bounding_box();

        chunk->align();
        chunk->write_vector(aabb.lower);
        chunk->write_vector(aabb.upper);
    }

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

    void flatten(std::vector<Hittable*>* storage) override {
        for(auto child : children) child->flatten(storage);
    }

    std::vector<Hittable*>& get_children() { return children; }
};