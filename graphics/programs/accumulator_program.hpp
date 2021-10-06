#pragma once

#include "../gl/uniform.hpp"
#include "../gl/gl_texture.hpp"
#include "../gl/gl_framebuffer.hpp"
#include "bounded_program.hpp"

namespace Graphics {

class TracerAccumulatorProgram : public BoundedProgram {
    Uniform old_texture_uniform;
    Uniform input_texture_uniform;
    Uniform frame_count_uniform;
    GLTexture* input_texture;
    GLTexture* final_texture;

    GLFramebuffer* framebuffer_a;
    GLFramebuffer* framebuffer_b;

    bool is_odd_phase = false;

    void bind_framebuffer() {
        if(is_odd_phase) {
            final_texture = framebuffer_a->get_texture();
            framebuffer_b->bind();
        } else {
            final_texture = framebuffer_b->get_texture();
            framebuffer_a->bind();
        }
    }

public:
    TracerAccumulatorProgram();

    void set_input_texture(GLTexture* p_input_texture) { input_texture = p_input_texture; }

    void draw(int frames) {
        use();

        bind_vao();

        bind_framebuffer();

        glActiveTexture(GL_TEXTURE0);
        input_texture->bind();
        input_texture_uniform.set1i(0);
        glActiveTexture(GL_TEXTURE1);
        final_texture->bind();
        old_texture_uniform.set1i(1);
        frame_count_uniform.set1i(frames);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        GLException::check();

        unbind_vao();
    }

    void next_frame() {
        is_odd_phase = !is_odd_phase;
    }

    void set_framebuffers(Graphics::GLFramebuffer* p_framebuffer_a, Graphics::GLFramebuffer* p_framebuffer_b) {
        framebuffer_a = p_framebuffer_a;
        framebuffer_b = p_framebuffer_b;
    }

    GLTexture* get_last_target_texture() {
        if(is_odd_phase) {
            return framebuffer_b->get_texture();
        } else {
            return framebuffer_a->get_texture();
        }
    }
};

}