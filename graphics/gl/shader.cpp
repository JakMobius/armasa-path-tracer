//
// Created by Артем on 29.09.2021.
//

#include "shader.hpp"
#include "../error_handling.hpp"
#include <sstream>
#include <iostream>
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
    shader_source_path << "./" << shader_path << name << ".shader";
    std::string shader_path = shader_source_path.str();
    std::ifstream shader_source_fstream(shader_path);

    if(!shader_source_fstream) {
//        std::cout << shader_path << "\n";
//        system("ls resources");
//        std::cout << "Error: " << strerror(errno);
        throw GLShaderCompileException("Could not open shader file");
    }

    std::stringstream shader_source_stream;
    shader_source_stream << "#version 410 core\n" << shader_source_fstream.rdbuf() << '\0';
    const char* shader_source = strdup(shader_source_stream.str().c_str());
    shader_source_fstream.close();

    const GLint lengths[] = { (GLint)strlen(shader_source) };
    const char* sources[] = { shader_source };

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

        throw GLShaderCompileException(info);
    }
}
}