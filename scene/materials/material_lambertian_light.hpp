#pragma once

#include "../../utils/vec3.hpp"
#include "material.hpp"
#include "../buffer_chunk.hpp"

extern const int MaterialLambertianLightType;

class MaterialLambertianLight : public Material {
    Vec3f color;
public:

    MaterialLambertianLight(const Vec3f& color): color(color) {
        set_index_buffer_stride(2);
        set_float_buffer_stride(3);
    }

    virtual void render(SceneRenderer* renderer, BufferChunk* chunk) override {
        chunk->write_index(MaterialLambertianLightType);
        chunk->write_float_buffer_index();
        chunk->write_vector(color);
    };

    void set_color(const Vec3f& p_color) { color = p_color; }
    const Vec3f& get_color() { return color; }
};