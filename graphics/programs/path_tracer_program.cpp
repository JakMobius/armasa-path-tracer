//
// Created by Артем on 29.09.2021.
//

#include "path_tracer_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

namespace Graphics {
PathTracerProgram::PathTracerProgram():
    VertexFragmentProgram("path_tracing/vertex", "path_tracing/fragment"),
    screen_size_uniform(this, "u_screen_size"),
    scene_float_buffer_uniform(this, "u_float_buffer"),
    camera_controller_uniform(this)
    //scene_index_buffer_uniform(this, "u_index_buffer")
    {

    vertex_buffer = new GLBuffer<float>(GLBufferType::array_buffer, GLBufferUsage::static_draw);
    vertex_buffer->get_storage().assign({-1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, 1});
    vertex_buffer->create_buffer();
    vertex_buffer->synchronize();

    scene_float_buffer = new GLTextureBuffer<float>(0, GLBufferUsage::dynamic_draw);
    for(int i = 0; i < 5; i++) {
        scene_float_buffer->get_storage().push_back(1.0 * (float)i / 5);
        scene_float_buffer->get_storage().push_back(0.5 * (float)i / 5);
        scene_float_buffer->get_storage().push_back(0.0 * (float)i / 5);
    }
    scene_float_buffer->create_buffer();
    scene_float_buffer->synchronize();

//    scene_index_buffer = new GLTextureBuffer<int>(1, GLBufferUsage::dynamic_draw);
//    scene_index_buffer->get_storage().assign({0, 0, 0, 0});
//    scene_index_buffer->create_buffer();
//    scene_index_buffer->synchronize();

    set_vao({
        {
            vertex_buffer, {
                { "a_position", 2 }
            }
        }
    });
}
}