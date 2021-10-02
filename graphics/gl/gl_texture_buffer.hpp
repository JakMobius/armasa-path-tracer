#pragma once

#include "gl_buffer.hpp"

namespace Graphics {

enum class GLTextureBufferType {
    R8 = GL_R8,
    R16 = GL_R16,
    R16F = GL_R16F,
    R32F = GL_R32F,
    R8I = GL_R8I,
    R16I = GL_R16I,
    R32I = GL_R32I,
    R8UI = GL_R8UI,
    R16UI = GL_R16UI,
    R32UI = GL_R32UI,
    RG8 = GL_RG8,
    RG16 = GL_RG16,
    RG16F = GL_RG16F,
    RG32F = GL_RG32F,
    RG8I = GL_RG8I,
    RG16I = GL_RG16I,
    RG32I = GL_RG32I,
    RG8UI = GL_RG8UI,
    RG16UI = GL_RG16UI,
    RG32UI = GL_RG32UI,
    RGB32F = GL_RGB32F,
    RGB32I = GL_RGB32I,
    RGB32UI = GL_RGB32UI,
    RGBA8 = GL_RGBA8,
    RGBA16 = GL_RGBA16,
    RGBA16F = GL_RGBA16F,
    RGBA32F = GL_RGBA32F,
    RGBA8I = GL_RGBA8I,
    RGBA16I = GL_RGBA16I,
    RGBA32I = GL_RGBA32I,
    RGBA8UI = GL_RGBA8UI,
    RGBA16UI = GL_RGBA16UI,
    RGBA32UI = GL_RGBA32UI
};

template<typename T>
class GLTextureBuffer : public GLBuffer<T> {
    GLuint texture_handle = -1;
    GLTextureBufferType type;
public:
    explicit GLTextureBuffer(GLTextureBufferType type, GLBufferUsage usage = GLBufferUsage::static_draw):
        GLBuffer<T>(GLBufferType::texture_buffer, usage),
        type(type) {
    };

    void create_buffer() override {
        GLBuffer<T>::create_buffer();
        glGenTextures(1, &texture_handle);
    }

    void bind_texture(GLenum texture_index) {
        glActiveTexture(texture_index);
        glBindTexture(GL_TEXTURE_BUFFER, texture_handle);
        glTexBuffer(GL_TEXTURE_BUFFER, (GLenum) type, this->get_handle());
    }

    GLuint get_texture_handle() {
        return texture_handle;
    }
};

}