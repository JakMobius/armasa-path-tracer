#pragma once

namespace Graphics {
class GLBufferBase;
}

#include <GL/glew.h>
#include <vector>
#include "../error_handling.hpp"
#include "gl_enums.hpp"

namespace Graphics {



class GLBufferBase {
protected:
    GLuint gl_buffer_handle = 0;
    long gl_buffer_capacity = -1;
    GLenum gl_type = GL_INT;
    int gl_size = 4;
    GLBufferType type;
    GLBufferUsage usage;
public:
    explicit GLBufferBase(GLBufferType type, GLBufferUsage usage = GLBufferUsage::static_draw): type(type), usage(usage) {}
    virtual ~GLBufferBase() = default;
    virtual void create_buffer() {
        glGenBuffers(1, &gl_buffer_handle);
        GLException::check();
    }

    virtual void synchronize() = 0;

    virtual void bind() { glBindBuffer((GLenum)type, gl_buffer_handle); }

    GLuint get_handle() const { return gl_buffer_handle; }
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
    if((long)(host_array.size() * sizeof(T)) != gl_buffer_capacity) {
        glBufferData((GLenum)type, host_array.size() * sizeof(T), &host_array[0], (GLenum)usage);
        gl_buffer_capacity = host_array.size() * sizeof(T);
    } else {
        glBufferSubData((GLenum)type, 0, host_array.size() * sizeof(T), &host_array[0]);
    }
    GLException::check();
}

}