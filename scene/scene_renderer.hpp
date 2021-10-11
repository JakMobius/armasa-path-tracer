#pragma once

class SceneRenderer;
class Material;
class Hittable;
class Scene;
class SceneBufferSerializable;

#include <vector>
#include <queue>
#include <set>
#include <map>
#include "scene_buffer.hpp"
#include "scene_buffer_serializable.hpp"

struct SerializableChunkData {
    int index_buffer_position;
    int float_buffer_position;

    void reset() {
        index_buffer_position = 0;
        float_buffer_position = 0;
    }
};

class SceneRenderer {
    SceneBuffer* scene_buffer;
    Hittable* bvh_root;

    std::map<Material*, SerializableChunkData> material_map;
    std::map<Hittable*, SerializableChunkData> hittable_map;

    std::queue<Hittable*>  hittable_render_queue;

    SerializableChunkData material_block_length = { -1, -1 };
    SerializableChunkData current_block_lengths = {-1, -1 };

    bool layout_valid = false;

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

    int get_material_index(Material* material) { return material_map.at(material).index_buffer_position / alignment; }
    int get_hittable_index(Hittable* hittable) { return hittable_map.at(hittable).index_buffer_position / alignment; }

    void render(SceneBuffer* buffer, Scene* scene);

    SceneBuffer* get_scene_buffer() { return scene_buffer; }

    void render_single(SceneBuffer* buffer, SceneBufferSerializable* serializable, SerializableChunkData* bounds);

    void allocate_buffers(SceneBufferSerializable* serializable);
};