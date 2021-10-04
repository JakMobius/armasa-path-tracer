//
// Created by Артем on 03.10.2021.
//

#include "present_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

namespace Graphics {
Graphics::PresentProgram::PresentProgram():
    VertexFragmentProgram("present/vertex", "present/fragment"),
    texture_uniform(this, "u_texture"),
    frame_count_uniform(this, "u_frames"),
    gamma_uniform(this, "u_gamma"),
    brightness_uniform(this, "u_brightness") {

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