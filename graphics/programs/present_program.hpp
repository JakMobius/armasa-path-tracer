#pragma once

#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "../gl/gl_texture.hpp"
#include "bounded_program.hpp"
#include <GL/glew.h>

namespace Graphics {

class TracerPostProcessingProgram : public BoundedProgram {
    Uniform texture_uniform;
    Uniform gamma_uniform;
    Uniform brightness_uniform;
    GLTexture* texture;

    float gamma = 0.7;
    float brightness = 3.0;

public:
    TracerPostProcessingProgram();

    void set_texture(GLTexture* p_input_texture) { texture = p_input_texture; }

    void draw() {
        use();

        bind_vao();

        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        texture_uniform.set1i(0);
        brightness_uniform.set1f(brightness);
        gamma_uniform.set1f(gamma);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        GLException::check();

        unbind_vao();
    }

    void set_gamma(float p_gamma) { gamma = p_gamma; }
    float get_gamma() { return gamma; }
    void set_brightness(float p_brightness) { brightness = p_brightness; }
    float get_brightness() { return brightness; }
};

}