#pragma once

class HittableList;
class BVHNode;

#include <vector>
#include "hittable.hpp"
#include "../scene_renderer.hpp"
#include "../buffer_chunk.hpp"

extern const int HittableListType;

class HittableList : public Hittable {
    std::vector<Hittable*> children;

public:
    HittableList(): children() {
        set_index_buffer_stride(3);
        set_float_buffer_stride(8);
    }

    void add_children(Hittable* hittable) {
        children.push_back(hittable);
        // Two fields + children indices
        set_index_buffer_stride(2 + children.size());
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
        chunk->write_float_buffer_index();

        AABB aabb = get_bounding_box();

        chunk->write_vector(aabb.lower);
        chunk->write_vector(aabb.upper);

        for(int i = 0; i < children_count; i++) {
            int children_index = renderer->get_hittable_index(children[i]);
            chunk->write_index(children_index);
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