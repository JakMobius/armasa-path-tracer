#pragma once

#include <cassert>
#include "scene_buffer.hpp"

class BufferChunk {
    SceneBuffer* buffer;

    int index_buffer_position;

    int index_buffer_length;

    int internal_index_position = 0;

    std::vector<int>& get_index_buffer() { return buffer->get_index_buffer()->get_storage(); }

public:

    BufferChunk(SceneBuffer* buffer, int index_buffer_position, int index_buffer_length):
        buffer(buffer),
        index_buffer_position(index_buffer_position),
        index_buffer_length(index_buffer_length) {}

    void write_index(int index) {
        assert(internal_index_position < index_buffer_length);
        get_index_buffer()[index_buffer_position + internal_index_position++] = index;
    }

    void write_float(float flt) {
        write_index(*((int*)&flt));
    }

    void align() {
        while(internal_index_position % 4) write_index(0);
    }

    void write_vector(const Vec3f& vector, bool skip_fourth_component = true) {
        write_float(vector[0]);
        write_float(vector[1]);
        write_float(vector[2]);
        if(skip_fourth_component) internal_index_position++;
    };
};