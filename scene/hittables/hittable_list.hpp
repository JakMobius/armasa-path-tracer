#pragma once

class HittableList;

#include <vector>
#include "hittable.hpp"

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

    void layout(std::queue<Hittable*>& queue) override {
        for(auto child : children) queue.push(child);
    }

    void render(int* index_buffer, float* float_buffer) {
        int index = get_gl_buffer_index();
        index_buffer[index] = HittableListType;
        index_buffer[index + 1] = children.size();

        for(int i = 0; i < children.size(); i++) {
            index_buffer[index + 2 + i] = children[i]->get_gl_buffer_index();
            children[i]->render(index_buffer, float_buffer);
        }
    }
};