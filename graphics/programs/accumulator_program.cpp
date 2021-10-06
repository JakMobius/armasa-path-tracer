//
// Created by Артем on 03.10.2021.
//

#include "accumulator_program.hpp"
#include "../gl/vertex_array_object_factory.hpp"

namespace Graphics {
Graphics::TracerAccumulatorProgram::TracerAccumulatorProgram():
        BoundedProgram("accumulator/vertex", "accumulator/fragment"),
        old_texture_uniform(this, "u_old_texture"),
        input_texture_uniform(this, "u_input_texture"),
        frame_count_uniform(this, "u_frame") {

    set_vao({
        {vertex_buffer, {
            { "a_position", 2 }
        }}
    });
}

}