#pragma once

namespace Graphics {
class Program;
}

#include <vector>
#include "shader.hpp"
#include "gl_buffer.hpp"

namespace Graphics {
struct VertexArrayObjectConfig;

class Program {
    std::vector<Shader*> shaders;
    GLuint vao = 0;
    GLuint handle = 0;

public:
    Program();
    virtual ~Program();
    Program(const Program& copy) = delete;
    Program(Program&& move) = delete;
    Program &operator=(const Program* assign) = delete;
    Program &operator=(Program &&move_assign) = delete;

    void add_shader(Shader* shader);
    void set_vao(const VertexArrayObjectConfig& vao_config);
    GLuint get_attribute_index(const std::string& str) const {
        GLuint loc = glGetAttribLocation(handle, str.c_str());
        GLException::check();
        return loc;
    }

    void link();
    virtual void draw() = 0;

    GLuint get_handle() const { return handle; }
    GLuint get_vao_handle() const;
};
}