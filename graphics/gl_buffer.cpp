//
// Created by Артем on 29.09.2021.
//

#include "gl_buffer.hpp"

namespace Graphics {

template<> void GLBuffer<double>    ::set_gl_type() { gl_size = 8; gl_type = GL_DOUBLE; }
template<> void GLBuffer<float>     ::set_gl_type() { gl_size = 4; gl_type = GL_FLOAT; }
template<> void GLBuffer<uint32_t>  ::set_gl_type() { gl_size = 4; gl_type = GL_UNSIGNED_INT; }
template<> void GLBuffer<int32_t>   ::set_gl_type() { gl_size = 4; gl_type = GL_INT; }
template<> void GLBuffer<uint16_t>  ::set_gl_type() { gl_size = 2; gl_type = GL_UNSIGNED_SHORT; }
template<> void GLBuffer<int16_t>   ::set_gl_type() { gl_size = 2; gl_type = GL_SHORT; }
template<> void GLBuffer<uint8_t>   ::set_gl_type() { gl_size = 1; gl_type = GL_UNSIGNED_BYTE; }
template<> void GLBuffer<int8_t>    ::set_gl_type() { gl_size = 1; gl_type = GL_BYTE; }

}