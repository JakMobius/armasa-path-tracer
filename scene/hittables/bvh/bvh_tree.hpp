#pragma once

class BVHTree;

#include <vector>
#include "bvh_node.hpp"

class BVHTree : public Hittable {
    std::vector<BVHNode> nodes;
    std::vector<Hittable*> leaves;
    int stride = 0;

public:
    explicit BVHTree(Hittable* hittable): nodes(), leaves() {
        hittable->flatten(&leaves);
        generate_bvh(0, 0, leaves.size(), &leaves);
        set_index_buffer_stride(stride * 4 + 4);
    }

    void register_hittables(SceneRenderer* renderer) override {
        for(auto leaf : leaves) renderer->enqueue_hittable_render(leaf);
    }

    void register_materials(SceneRenderer *renderer) override {
        for(auto leaf : leaves) leaf->register_materials(renderer);
    }

    virtual void render(SceneRenderer* renderer, BufferChunk* chunk) override {

        chunk->write_vector(nodes[0].bounding_box.lower, true);
        for(int i = 0; i < stride; i++) {
            BVHNode* node = &nodes[i];

            unsigned int flags = node->flags;
            if(flags & BVHNodeFlags::is_leaf) {
                flags |= renderer->get_hittable_index(node->leaf);
            }

            chunk->write_index((int) flags);

            Vec3f aabb_boundary = node->get_masked_aabb_vector();

            chunk->write_vector(aabb_boundary, false);
        }
    }

    BVHNode* get_node(int index) {
        if((int)nodes.size() <= index) {
            if(nodes.empty()) nodes.resize(16, {});
            else nodes.resize(nodes.size() * 2, {});
        }
        if(index + 1 > stride) stride = index + 1;
        return &nodes[index];
    }

    void update_aabb() override;
    void generate_bvh(int node_index, int from, int to, std::vector<Hittable*>* list);
};
