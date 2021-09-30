#pragma once

#include "../../gl/uniform.hpp"
#include "../../gl/program.hpp"
#include "../../../utils/vec3.hpp"
#include "../../camera.hpp"

namespace Graphics {
class CameraUniformController {

    Uniform camera_position_uniform;
    Uniform camera_width_vector_uniform;
    Uniform camera_height_vector_uniform;
    Uniform camera_focus_uniform;

public:
    CameraUniformController(const Program* program) :
            camera_position_uniform(program, "u_camera_position"),
            camera_width_vector_uniform(program, "u_camera_width_vector"),
            camera_height_vector_uniform(program, "u_camera_height_vector"),
            camera_focus_uniform(program, "u_camera_focus") {

    }

    void update_uniforms(Camera* camera);
};
}