//
// Created by Артем on 02.10.2021.
//

#include "scene_renderer.hpp"
#include "materials/material.hpp"
#include "hittables/hittable.hpp"
#include "hittables/hittable_list.hpp"
#include "hittables/bvh_node.hpp"

void SceneRenderer::enqueue_hittable_render(Hittable* hittable) {
    hittable_render_queue.push(hittable);
    hittable_map.insert({ hittable, material_block_length + hittable_block_length });

    hittable_block_length = align(hittable_block_length + hittable->get_gl_buffer_stride());
}

void SceneRenderer::render(SceneBuffer* buffer) {
    if(material_block_length < 0 || hittable_block_length < 0) layout();

    buffer->require_capacity(material_block_length + hittable_block_length);

    scene_buffer = buffer;

    buffer->set_entry_hittable_index(material_block_length / alignment);

    for(auto entry : material_map) {
        entry.first->render(this, entry.second);
    }
    for(auto entry : hittable_map) {
        entry.first->render(this, entry.second);
    }

    buffer->set_needs_synchronize();

    scene_buffer = nullptr;
}

void SceneRenderer::build_bvh() {
    bvh_root = target->get_root_hittable()->to_bvh_node();
    //((BVHNode*)bvh_root)->dump();
}

void SceneRenderer::layout() {
    build_bvh();

    hittable_map.clear();
    material_map.clear();

    material_block_length = 0;
    hittable_block_length = 0;

    while(!hittable_render_queue.empty()) {
        hittable_render_queue.pop();
    }

    bvh_root->register_materials(this);
    enqueue_hittable_render(bvh_root);

    while(!hittable_render_queue.empty()) {
        Hittable* next = hittable_render_queue.front();
        hittable_render_queue.pop();

        next->register_hittables(this);
    }
}

void SceneRenderer::register_material(Material* material) {
    if(material_map.find(material) == material_map.end()) {
        material_map.insert({material, material_block_length});
        material_block_length = align(material_block_length + material->get_gl_buffer_stride());
    }
}
