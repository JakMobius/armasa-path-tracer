#pragma once

#include "material.hpp"
#include "../../utils/vec3.hpp"

extern const int MaterialMetalType;

class MaterialMetal : public Material {
    Vec3f color;
    float roughness;
public:

    MaterialMetal(const Vec3f& color, float roughness): color(color), roughness(roughness) {
        set_gl_buffer_stride(4);
    }

    virtual void render(SceneRenderer* renderer, int index) override {

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();
        auto& float_buffer = scene_buffer->get_float_buffer()->get_storage();

        index_buffer[index]     = MaterialMetalType;
        float_buffer[index]     = color[0];
        float_buffer[index + 1] = color[1];
        float_buffer[index + 2] = color[2];
        float_buffer[index + 3] = roughness;
    };

    void set_color(const Vec3f& p_color) { color = p_color; }
    const Vec3f& get_color() { return color; }
    void set_roughness(float p_roughness) { roughness = p_roughness; }
    float get_roughness() { return roughness; }
};