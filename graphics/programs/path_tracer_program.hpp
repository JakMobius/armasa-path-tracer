#pragma once

#include <GL/glew.h>
#include "../gl/gl_buffer.hpp"
#include "../gl/gl_texture_buffer.hpp"
#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "camera_uniform_controller/camera_uniform_controller.hpp"
#include "../../scene/scene_buffer.hpp"
#include "bounded_program.hpp"
#include <random>

namespace Graphics {


class TracerProgram: public BoundedProgram {
    GLTextureBuffer<float>* random_buffer;
    Uniform screen_size_uniform;
    Uniform scene_float_buffer_uniform;
    Uniform scene_index_buffer_uniform;
    Uniform scene_random_buffer_uniform;
    Uniform random_buffer_length_uniform;
    Uniform entry_index_uniform;
    Uniform seed_uniform;
    Uniform sample_uniform;
    Uniform max_reflections_uniform;
    CameraUniformController camera_controller_uniform;
    Camera* camera;
    std::mt19937 random;

    int random_buffer_length = 1024;
    int samples = 1;
    int max_reflections = 4;
    int screen_width;
    int screen_height;

public:
    TracerProgram(int width, int height);

    void update_random_buffer();

    void draw(SceneBuffer* scene) {

        if(!camera) return;
        use();

        screen_size_uniform.set2f((float)screen_width, (float)screen_height);

        bind_vao();

        update_random_buffer();

        scene->synchronize_if_needed();
        scene->bind_buffers(GL_TEXTURE0, GL_TEXTURE1);

        random_buffer->synchronize();
        random_buffer->bind_texture(GL_TEXTURE2);

        scene_index_buffer_uniform.set1i(0);
        scene_float_buffer_uniform.set1i(1);
        scene_random_buffer_uniform.set1i(2);

        random_buffer_length_uniform.set1i(random_buffer_length);
        seed_uniform.set1ui(random());
        entry_index_uniform.set1i(scene->get_entry_hittable_index());
        sample_uniform.set1i(samples);
        max_reflections_uniform.set1i(max_reflections);

        camera_controller_uniform.update_uniforms(camera);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        GLException::check();

        unbind_vao();
    }

    Camera* get_camera() const { return camera; }
    void set_camera(Camera* p_camera) { camera = p_camera; }

    int get_samples() const { return samples; }
    void set_samples(int p_samples) { samples = p_samples; }

    int get_max_reflections() const { return max_reflections; }
    void set_max_reflections(int p_max_reflections) { max_reflections = p_max_reflections; }
};
}