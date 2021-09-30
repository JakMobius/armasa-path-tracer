#pragma once

#include "gl_buffer.hpp"

namespace Graphics {

template<typename T>
class GLTextureBuffer : public GLBuffer<T> {
    GLuint texture_handle = -1;
    GLenum texture_index = GL_TEXTURE0;
public:
    explicit GLTextureBuffer(int texture_index, GLBufferUsage usage = GLBufferUsage::static_draw): GLBuffer<T>(GLBufferType::texture_buffer, usage) {
        texture_index = GL_TEXTURE0 + texture_index;
    };

    void create_buffer() override {
        GLBuffer<T>::create_buffer();
        glGenTextures(1, &texture_handle);
    }

    void bind() override {
        GLBuffer<T>::bind();
        glActiveTexture(texture_index);
        glBindTexture(GL_TEXTURE_BUFFER, texture_handle);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, this->get_handle());
    }

    GLuint get_texture_handle() {
        return texture_handle;
    }
};

}