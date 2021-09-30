#pragma once

namespace Graphics {
struct VertexArrayObjectConfig;
struct VertexArrayObjectEntryConfig;
struct VertexAttributeConfig;
}

#include "program.hpp"
#include "gl_buffer.hpp"

namespace Graphics {
class VertexFragmentProgram : public Program {
public:
    VertexFragmentProgram(const std::string& vertex_shader_name, const std::string& fragment_shader_name) {
        auto vertexShader = new Shader(vertex_shader_name, ShaderType::vertex);
        auto fragmentShader = new Shader(fragment_shader_name, ShaderType::fragment);

        vertexShader->compile();
        fragmentShader->compile();

        add_shader(vertexShader);
        add_shader(fragmentShader);

        link();
    }
};
}