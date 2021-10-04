#pragma once

#include <GL/glew.h>
#include "gl_texture.hpp"
#include "../error_handling.hpp"

namespace Graphics {

class GLFramebuffer {
    GLenum handle = 0;
    GLTexture* texture = nullptr;

public:
    GLFramebuffer() {};
    ~GLFramebuffer() { if(handle) glDeleteFramebuffers(1, &handle); }

    void create_framebuffer() {
        glGenFramebuffers(1, &handle);
        GLException::check();
    }

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->get_handle(), 0);
        GLException::check();
    }
    static void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void set_texture(GLTexture* p_texture) {
        texture = p_texture;
        GLException::check();
    }

    GLTexture* get_texture() {
        return texture;
    }

    bool is_complete() {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
};

}