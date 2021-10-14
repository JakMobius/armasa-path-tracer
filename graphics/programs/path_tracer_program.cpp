//
// Created by Артем on 29.09.2021.
//

#include "path_tracer_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

namespace Graphics {
TracerProgram::TracerProgram(int width, int height):
    BoundedProgram("path_tracing/vertex", "path_tracing/fragment"),
    screen_size_uniform(this, "u_screen_size"),
    scene_index_buffer_uniform(this, "u_index_buffer"),
    scene_random_buffer_uniform(this, "u_random_buffer"),
    random_buffer_length_uniform(this, "u_random_buffer_length"),
    entry_index_uniform(this, "u_entry_index"),
    seed_uniform(this, "u_seed"),
    sample_uniform(this, "u_samples"),
    max_reflections_uniform(this, "u_max_reflections"),
    background_uniform(this, "u_background"),
    camera_controller_uniform(this),
    random(),
    screen_width(width),
    screen_height(height)
    {

    random_buffer = new GLTextureBuffer<float>(Graphics::GLTextureInternalFormat::rgb32f, Graphics::GLBufferUsage::dynamic_draw);
    random_buffer->create_buffer();

    set_vao({
        {
            vertex_buffer, {
                { "a_position", 2 }
            }
        }
    });
}

void TracerProgram::update_random_buffer() {
    auto& storage = random_buffer->get_storage();
    storage.clear();
    for(int i = 0; i < random_buffer_length * 3; i++) {
        storage.push_back((float)random() / (float)std::mt19937::max());
    }
}

}