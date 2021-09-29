#pragma once

#include <string>
#include <exception>
#include <OpenGL/gl3.h>
#include <SDL_opengl.h>

class GLException : public std::exception {
    GLenum error;

public:
    [[nodiscard]] const char * what() const noexcept override {
        switch(error) {
            case GL_INVALID_ENUM: return "GL Error: GL_INVALID_ENUM";
            case GL_INVALID_VALUE: return "GL Error: GL_INVALID_VALUE";
            case GL_INVALID_OPERATION: return "GL Error: GL_INVALID_OPERATION";
            case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL Error: GL_INVALID_FRAMEBUFFER_OPERATION";
            case GL_OUT_OF_MEMORY: return "GL Error: GL_OUT_OF_MEMORY";
            case GL_STACK_UNDERFLOW: return "GL Error: GL_STACK_UNDERFLOW";
            case GL_STACK_OVERFLOW: return "GL Error: GL_STACK_OVERFLOW";
            default: return "Unknown GL Error";
        }
    }

    explicit GLException(GLenum error): error(error) {}

public:
    static void check() {
        GLenum error = glGetError();
        if(error) throw GLException(error);
    }
};

class GLStringException : std::exception {
    std::string reason;

public:
    [[nodiscard]] const char * what() const noexcept override {
        return reason.c_str();
    }

    explicit GLStringException(const std::string &reason): reason(reason) {}
};

class GLProgramLinkException : public GLStringException {
public:
    explicit GLProgramLinkException(const std::string &reason): GLStringException(reason) {}
};
class GLShaderCompileException : public GLStringException {
public:
    explicit GLShaderCompileException(const std::string &reason): GLStringException(reason) {}
};