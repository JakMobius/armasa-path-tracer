#pragma once

#include <exception>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../graphics/gl/gl_framebuffer.hpp"
#include "../graphics/programs/path_tracer_program.hpp"
#include "../graphics/programs/accumulator_program.hpp"
#include "scene_renderer.hpp"
#include "../graphics/programs/present_program.hpp"
#include "../graphics/programs/texture_program.hpp"
#include <sstream>

class SceneDrawerUnavailableException : public std::exception {
public:
    [[nodiscard]] const char * what() const noexcept override { return "SceneDrawer could not setup context to draw the scene"; }
};

class SceneDrawer {

    int width;
    int height;
    int seed;
    int frames;
    bool query_screenshot = false;

    Graphics::Camera* camera;

    Graphics::GLFramebuffer* single_frame_buffer;
    Graphics::GLFramebuffer* accumulator_a;
    Graphics::GLFramebuffer* accumulator_b;
    Graphics::GLFramebuffer* result_image;

    Graphics::TracerProgram tracer_program;
    Graphics::TracerAccumulatorProgram accumulator_program;
    Graphics::TracerPostProcessingProgram post_processing_program;
    Graphics::TextureProgram present_program;

    SceneBuffer scene_buffer;
    SceneRenderer renderer;
    Scene* scene;

    int static_frame_substeps = 1;
    int dynamic_frame_substeps = 1;
    int maximum_reflections = 7;
    int frame_substeps;
    int frame_substep = 0;
    bool is_dynamic = false;
    bool needs_render = true;

public:

    SceneDrawer(const SceneDrawer& copy) = delete;

    SceneDrawer(Scene* scene, Graphics::Camera* camera, int width, int height);

    ~SceneDrawer() {
        delete single_frame_buffer;
        delete accumulator_a;
        delete accumulator_b;
        delete result_image;
    };

    void draw_chunk() {
        if(needs_render) {
            renderer.render(&scene_buffer, scene);
            needs_render = false;
        }
        float render_height = 2 / (float)(frame_substeps);
        float y_from = render_height * (float)frame_substep - 1;
        float y_to = y_from + render_height;

        tracer_program.update_render_bounds(y_from, y_to);
        accumulator_program.update_render_bounds(y_from, y_to);
        post_processing_program.update_render_bounds(y_from, y_to);

        frame_substep++;

        single_frame_buffer->bind();
        tracer_program.draw(&scene_buffer);
        accumulator_program.draw(frames);
        result_image->bind();
        post_processing_program.set_texture(get_output_texture());
        post_processing_program.draw();

        if(frame_substep >= frame_substeps) {
            accumulator_program.next_frame();
            frame_substep = 0;
            if(!is_dynamic) frames++;

            if(query_screenshot) {
                present_program.take_screenshot(seed, frames);
                query_screenshot = false;
            }
        }
    }

    void present() {
        Graphics::GLFramebuffer::unbind();
        present_program.draw();
    }

    Graphics::TracerProgram* get_tracer_program() { return &tracer_program; }
    Graphics::TracerAccumulatorProgram* get_accumulator_program() { return &accumulator_program; }
    Graphics::TracerPostProcessingProgram* get_post_processing_program() { return &post_processing_program; }

    long long get_frame_rays() {
        return (long long)tracer_program.get_samples() * (long long)width * (long long)height;
    }

    int get_frames() {
        return (int)frames;
    }

    int get_frame_substep() {
        return frame_substep;
    }

    void reset() {
        frame_substep = 0;
        frames = 1;
        seed = time(0);
    }

    void take_screenshot() {
        query_screenshot = true;
    }

    bool get_dynamic_mode() const { return is_dynamic; }

    void set_dynamic_mode(bool p_is_dynamic) {
        if(p_is_dynamic && !is_dynamic) {
            reset();
            tracer_program.set_max_reflections(-1);
            frame_substeps = dynamic_frame_substeps;
        }
        if(!p_is_dynamic && is_dynamic){
            reset();
            tracer_program.set_max_reflections(maximum_reflections);
            frame_substeps = static_frame_substeps;
        }

        is_dynamic = p_is_dynamic;
    }

    int get_static_frame_substeps() const { return static_frame_substeps; }
    void set_static_frame_substeps(int p_static_frame_substeps) {
        static_frame_substeps = p_static_frame_substeps;
        if(!is_dynamic) frame_substeps = static_frame_substeps;
    }

    int get_dynamic_frame_substeps() const { return dynamic_frame_substeps; }
    void set_dynamic_frame_substeps(int p_dynamic_frame_substeps) {
        dynamic_frame_substeps = p_dynamic_frame_substeps;
        if(is_dynamic) frame_substeps = dynamic_frame_substeps;
    }

    int get_max_reflections() const { return maximum_reflections; }
    void set_max_reflections(int p_max_reflections) { maximum_reflections = p_max_reflections; }

    Graphics::GLTexture* get_output_texture() { return accumulator_program.get_last_target_texture(); };
    Graphics::Camera* get_camera() { return camera; };
};