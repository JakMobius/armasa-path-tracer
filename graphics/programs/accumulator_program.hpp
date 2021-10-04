#pragma once

#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "../gl/gl_texture.hpp"
#include "../gl/gl_framebuffer.hpp"

namespace Graphics {

class AccumulatorProgram : public VertexFragmentProgram {
    GLBuffer<float>* vertex_buffer;
    Uniform old_texture_uniform;
    Uniform input_texture_uniform;
    Uniform clear_uniform;
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

        is_odd_phase = !is_odd_phase;
    }

public:
    AccumulatorProgram();

    ~AccumulatorProgram() {
        delete vertex_buffer;
    }

    void set_input_texture(GLTexture* p_input_texture) { input_texture = p_input_texture; }

    void draw(bool clear) {
        use();

        bind_vao();

        bind_framebuffer();

        glActiveTexture(GL_TEXTURE0);
        input_texture->bind();
        input_texture_uniform.set1i(0);
        glActiveTexture(GL_TEXTURE1);
        final_texture->bind();
        old_texture_uniform.set1i(1);
        clear_uniform.set1i(clear ? 1 : 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        GLException::check();

        unbind_vao();
    }

    void set_framebuffers(Graphics::GLFramebuffer* p_framebuffer_a, Graphics::GLFramebuffer* p_framebuffer_b) {
        framebuffer_a = p_framebuffer_a;
        framebuffer_b = p_framebuffer_b;
    }

    GLTexture* get_last_target_texture() {
        if(is_odd_phase) {
            return framebuffer_a->get_texture();
        } else {
            return framebuffer_b->get_texture();
        }
    }
};

}