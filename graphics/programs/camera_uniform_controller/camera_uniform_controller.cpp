//
// Created by Артем on 30.09.2021.
//

#include "camera_uniform_controller.hpp"

namespace Graphics {

void CameraUniformController::update_uniforms(Camera* camera) {
    camera_position_uniform.setVec3f(camera->get_position());
    camera_width_vector_uniform.setVec3f(camera->get_calculated_width_vector());
    camera_height_vector_uniform.setVec3f(camera->get_calculated_height_vector());
    camera_focus_uniform.setVec3f(camera->get_calculated_focus_vector());
}

}