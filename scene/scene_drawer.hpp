#pragma once

#include <exception>
#include "../graphics/gl/gl_framebuffer.hpp"
#include "../graphics/programs/path_tracer_program.hpp"
#include "../graphics/programs/accumulator_program.hpp"
#include "scene_renderer.hpp"

class SceneDrawerUnavailableException : public std::exception {
public:
    [[nodiscard]] const char * what() const noexcept override { return "SceneDrawer could not setup context to draw the scene"; }
};

class SceneDrawer {

    int width;
    int height;
    long long frames;

    Graphics::Camera* camera;

    Graphics::GLFramebuffer* temp_framebuffer;
    Graphics::GLFramebuffer* final_framebuffer_a;
    Graphics::GLFramebuffer* final_framebuffer_b;

    Graphics::PathTracerProgram path_tracer_program;
    Graphics::AccumulatorProgram accumulator_program;

    SceneBuffer scene_buffer;
    SceneRenderer renderer;
    Scene* scene;

public:

    SceneDrawer(const SceneDrawer& copy) = delete;

    SceneDrawer(Scene* scene, Graphics::Camera* camera, int width, int height);

    ~SceneDrawer() {
        delete temp_framebuffer;
        delete final_framebuffer_a;
        delete final_framebuffer_b;
    };

    void draw() {
        if(frames == 0) renderer.render(&scene_buffer, scene);
        temp_framebuffer->bind();
        path_tracer_program.draw(&scene_buffer);
        accumulator_program.draw(frames == 0);
        frames++;
    }

    Graphics::PathTracerProgram* get_path_tracer_program() { return &path_tracer_program; }
    Graphics::AccumulatorProgram* get_accumulator_program() { return &accumulator_program; }

    long long get_frame_rays() {
        return (long long)path_tracer_program.get_samples() * (long long)width * (long long)height;
    }

    long long get_frames() {
        return frames;
    }

    void reset() {
        frames = 0;
    }

    Graphics::GLTexture* get_output_texture() { return accumulator_program.get_last_target_texture(); };
    Graphics::Camera* get_camera() { return camera; };
};