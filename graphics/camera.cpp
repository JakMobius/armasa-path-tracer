
#include "camera.hpp"

namespace Graphics {
void Camera::recalculate_vectors() {
    float heading_sin = sinf(heading);
    float heading_cos = cosf(heading);
    float pitch_sin = sinf(pitch);
    float pitch_cos = cosf(pitch);

    Matrix4f pitch_matrix({
                                  pitch_cos, 0, pitch_sin,  0,
                                  0,         1, 0,          0,
                                  pitch_sin, 0, -pitch_cos, 0,
                                  0,         0,  0,         1
                          });

    Matrix4f heading_matrix({
                                    heading_cos, heading_sin,  0, 0,
                                    -heading_sin, heading_cos, 0, 0,
                                    0,           0,            1, 0,
                                    0,           0,            0, 1
                            });

    Matrix4f matrix = pitch_matrix * heading_matrix;

    calculated_focus_vector = Vec3f {focus_distance, 0, 0};
    calculated_width_vector = Vec3f {0, camera_width, 0};
    calculated_height_vector = Vec3f {0, 0, camera_height};

    calculated_focus_vector *= matrix;
    calculated_width_vector *= matrix;
    calculated_height_vector *= matrix;
}
}
