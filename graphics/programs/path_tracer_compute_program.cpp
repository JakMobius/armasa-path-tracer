//
// Created by Артем on 05.10.2021.
//

#include "path_tracer_compute_program.hpp"

namespace Graphics {

PathTracerComputeProgram::PathTracerComputeProgram(): output(GLTextureTarget::texture_2d, GLTextureInternalFormat::rgba32f) {
    output.create_texture();
    output.bind();
    output.set_format(GLTextureFormat::rgba);
    output.set_type(GLTextureType::type_float);
    output.set_image(500, 500);

    auto shader = new Shader("resources/path_tracing_compute", ShaderType::compute);

    shader->compile();

    add_shader(shader);

    link();
}

}
