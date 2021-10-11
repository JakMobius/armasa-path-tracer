#pragma once

#include "scene_buffer.hpp"

class BufferChunk {
    SceneBuffer* buffer;

    int index_buffer_position;
    int float_buffer_position;

    int index_buffer_length;
    int float_buffer_length;

    int internal_index_position = 0;
    int internal_float_position = 0;

    std::vector<int>& get_index_buffer() { return buffer->get_index_buffer()->get_storage(); }
    std::vector<float>& get_float_buffer() { return buffer->get_float_buffer()->get_storage(); }

public:

    BufferChunk(SceneBuffer* buffer, int index_buffer_position, int float_buffer_position, int index_buffer_length, int float_buffer_length):
        buffer(buffer),
        index_buffer_position(index_buffer_position),
        float_buffer_position(float_buffer_position),
        index_buffer_length(index_buffer_length),
        float_buffer_length(float_buffer_length) {}

    void write_index(int index) {
        assert(internal_index_position < index_buffer_length);
        get_index_buffer()[index_buffer_position + internal_index_position++] = index;
    }

    void write_float(float flt) {
        assert(internal_float_position < float_buffer_length);
        get_float_buffer()[float_buffer_position + internal_float_position++] = flt;
    }

    int get_float_buffer_index() {
        return float_buffer_position / 4;
    }

    void write_float_buffer_index() {
        write_index(get_float_buffer_index());
    }

    void write_vector(const Vec3f& vector, bool align = true) {
        write_float(vector[0]);
        write_float(vector[1]);
        write_float(vector[2]);
        if(align) internal_float_position++;
    };
};