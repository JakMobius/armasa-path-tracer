#pragma once

namespace Graphics {
class GLBufferBase;
}

#include <GL/glew.h>
#include <vector>
#include "error_handling.hpp"

namespace Graphics {

enum class GLBufferType {
    array_buffer              = GL_ARRAY_BUFFER,
//  atomic_counter_buffer     = GL_ATOMIC_COUNTER_BUFFER,
//  copy_read_buffer          = GL_COPY_READ_BUFFER,
//  copy_write_buffer         = GL_COPY_WRITE_BUFFER,
//  dispatch_indirect_buffer  = GL_DISPATCH_INDIRECT_BUFFER,
//  draw_indirect_buffer      = GL_DRAW_INDIRECT_BUFFER,
    element_array_buffer      = GL_ELEMENT_ARRAY_BUFFER,
    pixel_pack_buffer         = GL_PIXEL_PACK_BUFFER,
    pixel_unpack_buffer       = GL_PIXEL_UNPACK_BUFFER,
//  query_buffer              = GL_QUERY_BUFFER,
//  shader_storage_buffer     = GL_SHADER_STORAGE_BUFFER,
//  texture_buffer            = GL_TEXTURE_BUFFER,
//  transform_feedback_buffer = GL_TRANSFORM_FEEDBACK_BUFFER,
//  uniform_buffer            = GL_UNIFORM_BUFFER,
};

enum class GLBufferUsage {
    static_draw  = GL_STATIC_DRAW,
    dynamic_draw = GL_DYNAMIC_DRAW
};



class GLBufferBase {
protected:
    GLuint gl_buffer_handle = 0;
    unsigned long gl_buffer_capacity = 0;
    GLenum gl_type = GL_INT;
    int gl_size = 4;
    GLBufferType type;
    GLBufferUsage usage;
public:
    explicit GLBufferBase(GLBufferType type, GLBufferUsage usage = GLBufferUsage::static_draw): type(type), usage(usage) {}
    virtual ~GLBufferBase() = default;
    void create_buffer() {
        glGenBuffers(1, &gl_buffer_handle);
        GLException::check();
    }

    virtual void synchronize() = 0;

    void bind() { glBindBuffer((GLenum)type, gl_buffer_handle); }

    GLenum get_gl_type() const { return gl_type; }
    int get_gl_size() const  { return gl_size; }
};

template<typename T>
class GLBuffer : public GLBufferBase {
    std::vector<T> host_array;
public:
    explicit GLBuffer(GLBufferType type, GLBufferUsage usage = GLBufferUsage::static_draw): GLBufferBase(type, usage), host_array() {
        set_gl_type();
    };

    void synchronize() override;
    void set_gl_type();

    std::vector<T>& get_storage() { return host_array; }
    const std::vector<T>& get_storage() const { return host_array; };
};

template<typename T>
void GLBuffer<T>::synchronize() {
    glBindBuffer((GLenum)type, gl_buffer_handle);
    if(host_array.size() * sizeof(T) != gl_buffer_capacity) {
        glBufferData((GLenum)type, host_array.size() * sizeof(T), &host_array[0], (GLenum)usage);
        gl_buffer_capacity = host_array.size() * sizeof(T);
    } else {
        glBufferSubData((GLenum)type, 0, host_array.size() * sizeof(T), &host_array[0]);
    }
    GLException::check();
}

}