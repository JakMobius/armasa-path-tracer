#pragma once

#include "gl_buffer.hpp"
#include "gl_texture.hpp"

namespace Graphics {

template<typename T>
class GLTextureBuffer : public GLBuffer<T> {
    GLTexture texture;
    GLTextureInternalFormat type;
public:
    explicit GLTextureBuffer(GLTextureInternalFormat type, GLBufferUsage usage = GLBufferUsage::static_draw):
        GLBuffer<T>(GLBufferType::texture_buffer, usage),
        texture(GLTextureTarget::texture_buffer, type),
        type(type) {
    };

    void create_buffer() override {
        GLBuffer<T>::create_buffer();
        texture.create_texture();
    }

    void bind_texture(GLenum texture_index) {
        glActiveTexture(texture_index);
        texture.bind();
        glTexBuffer((GLenum) GLTextureTarget::texture_buffer, (GLenum) type, this->get_handle());
    }

    GLTexture* get_texture() { return &texture; }
};

}