//
// Created by Артем on 29.09.2021.
//

#include "shader.hpp"
#include "../error_handling.hpp"
#include <sstream>
#include <fstream>

namespace Graphics {

const char* Shader::shader_path = "resources/shaders/";

Shader::Shader(const std::string& name, ShaderType type): type(type), name(name) {}

Shader::~Shader() {
    if(handle != 0) {
        glDeleteShader(handle);
    }
}

void Shader::compile() {
    std::stringstream shader_source_path;
    shader_source_path << shader_path << name << ".shader";
    std::string shader_path = shader_source_path.str();
    std::ifstream shader_source_fstream(shader_path);

    std::stringstream shader_source_stream;
    shader_source_stream << shader_source_fstream.rdbuf() << '\0';
    shader_source_fstream.close();

    std::string shader_source = shader_source_stream.str().c_str();

    const GLint lengths[] = { (GLint)shader_source.size() };
    const char* sources[] = { shader_source.c_str() };

    handle = glCreateShader((GLenum)type);
    GLException::check();

    glShaderSource(handle, 1, sources, lengths);
    GLException::check();

    glCompileShader(handle);
    GLException::check();

    GLint isCompiled = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);

    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

        std::string info(maxLength, '\0');
        glGetShaderInfoLog(handle, maxLength, &maxLength, &info[0]);

        glDeleteShader(handle);

        throw GLShaderCompileException(info);
    }
}
}