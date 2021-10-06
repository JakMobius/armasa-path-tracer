//
// Created by Артем on 29.09.2021.
//

#include "texture_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

Graphics::TextureProgram::TextureProgram():
    VertexFragmentProgram("texture/vertex", "texture/fragment"),
    texture_uniform(this, "u_texture") {
    vertex_buffer = new GLBuffer<float>(GLBufferType::array_buffer, GLBufferUsage::static_draw);
    vertex_buffer->get_storage().assign({-1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, 1});
    vertex_buffer->create_buffer();
    vertex_buffer->synchronize();

    set_vao({
        {
            vertex_buffer, {
                { "a_position", 2 }
            }
        }
    });
}
