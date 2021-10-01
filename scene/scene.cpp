//
// Created by Артем on 01.10.2021.
//

#include "scene.hpp"
#include "hittables/hittable_list.hpp"

Scene::Scene() {
    root_hittable = new HittableList();
}

void Scene::render(Graphics::GLTextureBuffer<int>* index_buffer, Graphics::GLTextureBuffer<float>* float_buffer) {

    if(data_length < 0) {
        layout_buffer();
    }

    auto& float_buffer_storage = float_buffer->get_storage();
    auto& index_buffer_storage = index_buffer->get_storage();

    if((int)float_buffer_storage.size() < data_length) {
        float_buffer_storage.resize(data_length);
    }

    if((int)index_buffer_storage.size() < data_length) {
        index_buffer_storage.resize(data_length);
    }

    root_hittable->render(&index_buffer_storage[0], &float_buffer_storage[0]);
}

void Scene::layout_buffer() {
    data_length = 0;
    std::queue<Hittable*> queue;

    queue.push(root_hittable);

    while(!queue.empty()) {
        Hittable* first = queue.front();

        if(!first->get_scene() || first->get_scene() != this) {
            first->set_scene(this);
        }

        first->set_gl_buffer_index(data_length);
        data_length += first->get_gl_buffer_stride();
        queue.pop();
        first->layout(queue);
    }
}

Scene::~Scene() {
    delete root_hittable;
}
