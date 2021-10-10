#pragma once

#include <GL/glew.h>
#include "gl_enums.hpp"
#include "../error_handling.hpp"
#include <vector>

namespace Graphics {

class GLTexture {
    GLuint handle = 0;
    GLTextureTarget target;
    GLTextureInternalFormat internal_format;
    GLTextureFormat format = GLTextureFormat::rgba;
    GLTextureType type = GLTextureType::type_byte;
    int width = -1;
    int height = -1;

public:
    GLTexture(GLTextureTarget target, GLTextureInternalFormat internal_format):
            target(target),
            internal_format(internal_format)
    {};
    ~GLTexture() { if(handle) glDeleteTextures(1, &handle); }

    void create_texture() {
        glGenTextures(1, &handle);
        GLException::check();
    }

    void set_format(GLTextureFormat p_format) { format = p_format; }
    void set_type(GLTextureType p_type) { type = p_type; }

    void bind() {
        glBindTexture((GLenum)target, (GLenum) handle);
        GLException::check();
    }

    void set_image(int p_width, int p_height, const void *pixels = nullptr) {
        width = p_width;
        height = p_height;
        glTexImage2D((GLenum)target, 0, (GLint)internal_format, p_width, p_height, 0, (GLenum)format, (GLenum)type, pixels);
        GLException::check();
    };

    void set_parameter(GLTextureParameter parameter, GLint value) {
        glTexParameteri((GLenum)target, (GLenum)parameter, value);
        GLException::check();
    }

    void set_parameter(GLTextureParameter parameter, GLfloat value) {
        glTexParameterf((GLenum)target, (GLenum)parameter, value);
        GLException::check();
    }

    void* download(GLTextureFormat p_format, GLTextureType p_type) {

        if(width < 0 || height < 0) return nullptr;

        void* buffer = calloc(width * height, get_texture_format_size(format) * get_texture_type_size(type));
        glGetTexImage((GLenum) target, 0, (GLenum) p_format, (GLenum) p_type, buffer);

        GLenum error = glGetError();
        if(error) {
            free(buffer);
            throw GLException(error);
        }

        return buffer;
    }

    GLuint get_handle() const { return handle; }
    GLTextureTarget get_target() const { return target; }

    static int get_texture_format_size(GLTextureFormat format) {
        switch(format) {
            case GLTextureFormat::red:
            case GLTextureFormat::stencil_index:
            case GLTextureFormat::depth_component:
                return 1;

            case GLTextureFormat::rg:
            case GLTextureFormat::depth_stencil:
                return 2;

            case GLTextureFormat::rgb:
            case GLTextureFormat::bgr:
            case GLTextureFormat::red_integer:
            case GLTextureFormat::rg_integer:
            case GLTextureFormat::rgb_integer:
            case GLTextureFormat::bgr_integer:
                return 3;

            case GLTextureFormat::rgba_integer:
            case GLTextureFormat::bgra_integer:
            case GLTextureFormat::rgba:
            case GLTextureFormat::bgra:
                return 4;
        }
    }

    static int get_texture_type_size(GLTextureType type) {
        switch(type) {
            case GLTextureType::type_unsigned_byte:
            case GLTextureType::type_unsigned_byte_3_3_2:
            case GLTextureType::type_unsigned_byte_2_3_3_rev:
            case GLTextureType::type_byte:
                return 1;

            case GLTextureType::type_unsigned_short:
            case GLTextureType::type_short:
            case GLTextureType::type_unsigned_short_5_6_5:
            case GLTextureType::type_unsigned_short_5_6_5_rev:
            case GLTextureType::type_unsigned_short_4_4_4_4:
            case GLTextureType::type_unsigned_short_4_4_4_4_rev:
            case GLTextureType::type_unsigned_short_5_5_5_1:
            case GLTextureType::type_unsigned_short_1_5_5_5_rev:
            case GLTextureType::type_half_float:
                return 2;

            case GLTextureType::type_unsigned_int:
            case GLTextureType::type_int:
            case GLTextureType::type_float:
            case GLTextureType::type_unsigned_int_8_8_8_8:
            case GLTextureType::type_unsigned_int_8_8_8_8_rev:
            case GLTextureType::type_unsigned_int_10_10_10_2:
            case GLTextureType::type_unsigned_int_2_10_10_10_rev:
                return 4;
        }
    }

    int get_width() { return width; }
    int get_height() { return width; }
};

}