//
// Created by Артем on 01.10.2021.
//

#include "scene.hpp"
#include "hittables/hittable_list.hpp"
#include "scene_buffer.hpp"

Scene::Scene() {
    root_hittable = new HittableList();
}

Scene::~Scene() {
    delete root_hittable;
}
