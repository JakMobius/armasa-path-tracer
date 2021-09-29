#pragma once

#include <SDL.h>
#include <OpenGL/gl3.h>
#include "program.hpp"
#include <vector>
#include <string>

namespace Graphics {
struct VertexAttributeConfig {
    std::string name;
    int size;
    bool normalized;

    VertexAttributeConfig(const std::string &name, int size, bool normalized = false) : name(name), size(size),
                                                                                        normalized(normalized) {};
};

struct VertexArrayObjectEntryConfig {
    GLBufferBase* buffer;
    std::vector<VertexAttributeConfig> attributes;

    VertexArrayObjectEntryConfig(GLBufferBase* buffer,
                                 const std::initializer_list<VertexAttributeConfig> &attribute_configs) :
            buffer(buffer), attributes(attribute_configs) {};

    void create_entry(const Program &program) const {

        buffer->bind();

        int stride = 0;
        for(auto &attribute: attributes) stride += attribute.size * buffer->get_gl_size();

        int offset = 0;
        for(auto &attribute: attributes) {
            GLuint index = program.get_attribute_index(attribute.name);
            glVertexAttribPointer(
                index,
                attribute.size,
                buffer->get_gl_type(),
                attribute.normalized,
                stride,
                (const GLvoid *)(long long)(offset)
            );
            GLException::check();

            glEnableVertexAttribArray(index);
            GLException::check();

            offset += attribute.size * buffer->get_gl_size();
        }
    }
};

struct VertexArrayObjectConfig {
    std::vector<VertexArrayObjectEntryConfig> entry_configs;

    VertexArrayObjectConfig(const std::initializer_list<VertexArrayObjectEntryConfig> &entry_configs) :
            entry_configs(entry_configs) {};

    GLuint create_vao(const Program &program) const {
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        GLException::check();
        glBindVertexArray(vao);

        for(auto &entry: entry_configs) entry.create_entry(program);

        glBindVertexArray(0);
        GLException::check();
        return vao;
    }
};
}