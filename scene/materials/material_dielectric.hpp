#pragma once

#include "material.hpp"
#include "../buffer_chunk.hpp"

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
        set_index_buffer_stride(12);
    }

    virtual void render(SceneRenderer*, BufferChunk* chunk) override {

        chunk->write_index(MaterialDielectricType);

        chunk->align();
        chunk->write_vector(color);
        chunk->write_float(roughness);
        chunk->write_float(refr_coef);
        chunk->write_float(fuzziness);
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