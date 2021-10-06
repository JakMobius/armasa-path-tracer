#pragma once

#include "material.hpp"

extern const int MaterialDielectricType;

class MaterialDielectric : public Material {
    Vec3f color;
    float roughness;
    float refr_coef;
    float fuzziness;
public:

    MaterialDielectric(const Vec3f& color, float refr_coef = 1, float roughness = -1, float fuzziness = 0):
        color(color),
        roughness(roughness),
        refr_coef(refr_coef),
        fuzziness(fuzziness) {
        set_gl_buffer_stride(8);
    }

    virtual void render(SceneRenderer* renderer, int index) override {

        auto scene_buffer = renderer->get_scene_buffer();
        auto& index_buffer = scene_buffer->get_index_buffer()->get_storage();
        auto& float_buffer = scene_buffer->get_float_buffer()->get_storage();

        index_buffer[index]     = MaterialDielectricType;
        scene_buffer->write_vector(color, index);
        float_buffer[index + 4] = roughness;
        float_buffer[index + 5] = refr_coef;
        float_buffer[index + 6] = fuzziness;
    };

    void set_color(const Vec3f& p_color) { color = p_color; }
    const Vec3f& get_color() { return color; }

    void set_roughness(float p_roughness) { roughness = p_roughness; }
    float get_roughness() { return roughness; }
    void set_refr_coef(float p_refr_coef) { refr_coef = p_refr_coef; }
    float get_refr_coef() { return refr_coef; }
    void set_fuzziness(float p_fuzziness) { fuzziness = p_fuzziness; }
    float get_fuzziness() { return fuzziness; }
};