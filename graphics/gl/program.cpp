//
// Created by Артем on 29.09.2021.
//

#include <GL/glew.h>
#include "program.hpp"
#include "vertex_array_object_factory.hpp"

namespace Graphics {

Program::Program(): shaders() {}

Program::~Program() {
    for(auto shader : shaders) delete shader;
}

void Program::add_shader(Shader* shader) {
    shaders.push_back(shader);
}

void Program::link() {
    handle = glCreateProgram();
    GLException::check();

    for(auto shader : shaders) {
        if(!shader->get_handle()) throw GLException(GL_INVALID_OPERATION);
        glAttachShader(handle, shader->get_handle());
        GLException::check();
    }

    glLinkProgram(handle);
    GLException::check();

    for(auto shader : shaders) {
        glDetachShader(handle, shader->get_handle());
        GLException::check();
    }

    GLint isLinked = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

        std::string info(maxLength, '\0');
        glGetProgramInfoLog(handle, maxLength, &maxLength, &info[0]);

        throw GLProgramLinkException(info);
    }
}

void Program::set_vao(const VertexArrayObjectConfig &vao_config) {
    if(!handle) throw GLException(GL_INVALID_OPERATION);
    this->vao = vao_config.create_vao(*this);
}

GLuint Program::get_vao_handle() const {
    return this->vao;
}

}