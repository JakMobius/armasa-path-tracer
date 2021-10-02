#pragma once

#include "../../utils/vec3.hpp"
#include "material.hpp"

extern const int MaterialLambertianLightType;

class MaterialLambertianLight : public Material {
    Vec3f color;
    Vec3f light_color;
public:

    MaterialLambertianLight(const Vec3f& color, const Vec3f& light_color): color(color), light_color(light_color) {
        set_gl_buffer_stride(6);
    }

    virtual void render(SceneRenderer* renderer, int index) override {

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();
        auto& float_buffer = scene_buffer->get_float_buffer()->get_storage();

        index_buffer[index]     = MaterialLambertianLightType;
        float_buffer[index]     = color[0];
        float_buffer[index + 1] = color[1];
        float_buffer[index + 2] = color[2];
        float_buffer[index]     = light_color[0];
        float_buffer[index + 1] = light_color[1];
        float_buffer[index + 2] = light_color[2];
    };

    void set_color(const Vec3f& p_color) { color = p_color; }
    const Vec3f& get_color() { return color; }
    void set_light_color(const Vec3f& p_light_color) { light_color = p_light_color; }
    const Vec3f& get_light_color() { return light_color; }
};