//
// Created by Артем on 29.09.2021.
//

#include "triangle_program.hpp"
#include "../vertex_array_object_factory.hpp"

Graphics::TriangleProgram::TriangleProgram(): VertexFragmentProgram("triangle/vertex", "triangle/fragment") {
    buffer = new GLBuffer<float>(GLBufferType::array_buffer, GLBufferUsage::static_draw);
    buffer->create_buffer();

    set_vao({
        {
            buffer, {
                { "a_position", 2 }
            }
        }
    });
}
