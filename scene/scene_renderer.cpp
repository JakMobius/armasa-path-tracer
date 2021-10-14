//
// Created by Артем on 02.10.2021.
//

#include "scene_renderer.hpp"
#include "materials/material.hpp"
#include "hittables/hittable.hpp"
#include "hittables/hittable_list.hpp"
#include "hittables/bvh/bvh_tree.hpp"

void SceneRenderer::allocate_buffers(SceneBufferSerializable* serializable) {
    int index_stride = serializable->get_index_buffer_stride();

    current_block_lengths.index_buffer_position = align(current_block_lengths.index_buffer_position + index_stride);
}

void SceneRenderer::enqueue_hittable_render(Hittable* hittable) {
    hittable_render_queue.push(hittable);

    hittable_map.insert({ hittable, current_block_lengths });

    allocate_buffers(hittable);
}

void SceneRenderer::render_single(SceneBuffer* buffer, SceneBufferSerializable* serializable, SerializableChunkData* bounds) {
    BufferChunk chunk(
        buffer,
        bounds->index_buffer_position,
        serializable->get_index_buffer_stride()
    );
    serializable->render(this, &chunk);
}

void SceneRenderer::render(SceneBuffer* buffer, Scene* scene) {
    if(!layout_valid) layout(scene);

    buffer->require_index_buffer_capacity(current_block_lengths.index_buffer_position);

    scene_buffer = buffer;

    buffer->set_entry_hittable_index(material_block_length.index_buffer_position / alignment);

    for(auto entry : material_map) render_single(buffer, entry.first, &entry.second);
    for(auto entry : hittable_map) render_single(buffer, entry.first, &entry.second);

    buffer->set_needs_synchronize();

    scene_buffer = nullptr;
}

void SceneRenderer::layout(Scene* scene) {
    bvh_root = new BVHTree(scene->get_root_hittable());

    hittable_map.clear();
    material_map.clear();

    material_block_length.reset();
    current_block_lengths.reset();

    while(!hittable_render_queue.empty()) {
        hittable_render_queue.pop();
    }

    bvh_root->register_materials(this);

    material_block_length = current_block_lengths;

    enqueue_hittable_render(bvh_root);

    while(!hittable_render_queue.empty()) {
        Hittable* next = hittable_render_queue.front();
        hittable_render_queue.pop();

        next->register_hittables(this);
    }

    layout_valid = true;
}

void SceneRenderer::register_material(Material* material) {
    if(material_map.find(material) == material_map.end()) {
        material_map.insert({material, current_block_lengths});

        allocate_buffers(material);
    }
}
