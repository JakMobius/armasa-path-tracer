#pragma once

#include <SDL.h>
#include <OpenGL/gl3.h>
#include "../vertex_fragment_program.hpp"

namespace Graphics {

struct TriangleShape {
    float x1, y1;
    float x2, y2;
    float x3, y3;
};

class TriangleProgram : public VertexFragmentProgram {
    int vertices = 0;
    bool should_resend = false;
    GLBuffer<float>* buffer;
public:
    TriangleProgram();

    ~TriangleProgram() {
        delete buffer;
    }

    void addTriangle(const TriangleShape& shape) {
        std::vector<float>& storage = buffer->get_storage();
        storage.push_back(shape.x1);
        storage.push_back(shape.y1);
        storage.push_back(shape.x2);
        storage.push_back(shape.y2);
        storage.push_back(shape.x3);
        storage.push_back(shape.y3);

        vertices += 3;

        should_resend = true;
    }

    void draw() override {
        glUseProgram(get_handle());

        if(should_resend) buffer->synchronize();
        should_resend = false;

        glBindVertexArray(get_vao_handle());
        GLException::check();

        glDrawArrays(GL_TRIANGLES, 0, vertices);
        GLException::check();

        glBindVertexArray(0);
    }
};
}