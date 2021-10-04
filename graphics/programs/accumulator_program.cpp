//
// Created by Артем on 03.10.2021.
//

#include "accumulator_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

namespace Graphics {
Graphics::AccumulatorProgram::AccumulatorProgram():
        VertexFragmentProgram("accumulator/vertex", "accumulator/fragment"),
        old_texture_uniform(this, "u_old_texture"),
        input_texture_uniform(this, "u_input_texture"),
        clear_uniform(this, "u_clear") {

    vertex_buffer = new GLBuffer<float>(GLBufferType::array_buffer, GLBufferUsage::static_draw);
    vertex_buffer->get_storage().assign({-1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, 1});
    vertex_buffer->create_buffer();
    vertex_buffer->synchronize();

    set_vao({
        {vertex_buffer, {
            { "a_position", 2 }
        }}
    });
}

}