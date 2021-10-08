#pragma once

#include "../utils/vec3.hpp"

namespace Graphics {
class Camera {

    Vec3f position {0, 0, 0};
    float camera_width = 1;
    float camera_height = 1;
    float focus_distance = 1;

    bool should_update_vectors = true;
    Vec3f calculated_width_vector {1, 0, 0};
    Vec3f calculated_height_vector {0, 1, 0};
    Vec3f calculated_focus_vector {0, 0, 1};

    void recalculate_vectors();

public:

    Camera() = default;
    ~Camera() = default;

    const Vec3f &get_position() const { return position; }
    float get_camera_width() const { return camera_width; }
    float get_camera_height() const { return camera_height; }
    float get_focus_distance() const { return focus_distance; }

    void set_position(const Vec3f& p_position) { position = p_position; should_update_vectors = true; }
    void set_camera_width(float p_camera_width) { camera_width = p_camera_width; should_update_vectors = true; }
    void set_camera_height(float p_camera_height) { camera_height = p_camera_height; should_update_vectors = true; }
    void set_focus_distance(float p_focus_distance) { focus_distance = p_focus_distance; should_update_vectors = true; }

    const Vec3f& get_calculated_width_vector() {
        if(should_update_vectors) recalculate_vectors();
        return calculated_width_vector;
    }

    const Vec3f& get_calculated_height_vector() {
        if(should_update_vectors) recalculate_vectors();
        return calculated_height_vector;
    }

    const Vec3f& get_calculated_focus_vector() {
        if(should_update_vectors) recalculate_vectors();
        return calculated_focus_vector;
    }

    bool is_moved() { return should_update_vectors; }
    void set_moved() { should_update_vectors = true; }

    Matrix4f matrix = {};
};
}