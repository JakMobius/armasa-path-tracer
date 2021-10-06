#pragma once

class SceneRenderer;
class Material;
class Hittable;
class Scene;

#include <vector>
#include <queue>
#include <set>
#include <map>
#include "scene_buffer.hpp"

class SceneRenderer {
    SceneBuffer* scene_buffer;
    Hittable* bvh_root;

    std::map<Material*, int> material_map;
    std::map<Hittable*, int> hittable_map;

    std::queue<Hittable*>  hittable_render_queue;

    int material_block_length = -1;
    int hittable_block_length = -1;
    int alignment = 4;

    int align(int size) const {
        if(size % alignment > 0) {
            size += alignment - (size % alignment);
        }
        return size;
    }

    void layout(Scene* target);

public:
    explicit SceneRenderer(): material_map(), hittable_map() {};

    void register_material(Material* material);

    void enqueue_hittable_render(Hittable* hittable);

    int get_material_index(Material* material) { return material_map.at(material) / alignment; }
    int get_hittable_index(Hittable* hittable) { return hittable_map.at(hittable) / alignment; }

    void render(SceneBuffer* buffer, Scene* scene);

    SceneBuffer* get_scene_buffer() { return scene_buffer; }
};