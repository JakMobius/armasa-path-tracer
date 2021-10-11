#pragma once

#include "material.hpp"
#include "../../utils/vec3.hpp"
#include "../buffer_chunk.hpp"

extern const int MaterialLambertianType;

class MaterialLambertian : public Material {
    Vec3f color;
public:

    MaterialLambertian(const Vec3f& color): color(color) {
        set_index_buffer_stride(2);
        set_float_buffer_stride(3);
    }

    virtual void render(SceneRenderer* renderer, BufferChunk* chunk) override {
        chunk->write_index(MaterialLambertianType);
        chunk->write_float_buffer_index();
        chunk->write_vector(color);
    };

    void set_color(const Vec3f& p_color) { color = p_color; }
    const Vec3f& get_color() { return color; }
};