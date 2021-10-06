#pragma once

#include "../gl/vertex_fragment_program.hpp"

namespace Graphics {

class BoundedProgram : public VertexFragmentProgram {
protected:
    GLBuffer<float>* vertex_buffer;

    float boundary_y_from = -1;
    float boundary_y_to = 1;
public:

    BoundedProgram(const std::string& vertex_shader_name, const std::string& fragment_shader_name):
        VertexFragmentProgram(vertex_shader_name, fragment_shader_name) {
        vertex_buffer = new GLBuffer<float>(GLBufferType::array_buffer, GLBufferUsage::static_draw);
        vertex_buffer->get_storage().assign({-1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, 1});
        vertex_buffer->create_buffer();
        vertex_buffer->synchronize();
    }

    ~BoundedProgram() {
        delete vertex_buffer;
    }

    void update_render_bounds(float p_y_from, float p_y_to) {
        boundary_y_from = p_y_from;
        boundary_y_to = p_y_to;

        vertex_buffer->bind();
        vertex_buffer->get_storage().assign({-1, boundary_y_from, 1, boundary_y_from, 1, boundary_y_to, -1, boundary_y_from, -1, boundary_y_to, 1, boundary_y_to});
        vertex_buffer->synchronize();
    }

};
}