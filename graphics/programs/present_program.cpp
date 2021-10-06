//
// Created by Артем on 03.10.2021.
//

#include "present_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

namespace Graphics {
Graphics::TracerPostProcessingProgram::TracerPostProcessingProgram():
    BoundedProgram("post_process/vertex", "post_process/fragment"),
    texture_uniform(this, "u_texture"),
    gamma_uniform(this, "u_gamma"),
    brightness_uniform(this, "u_brightness") {

    set_vao({
        {vertex_buffer, {
            { "a_position", 2 }
        }}
    });
}
}