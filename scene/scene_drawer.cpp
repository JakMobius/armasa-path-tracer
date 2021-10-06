//
// Created by Артем on 06.10.2021.
//

#include "scene_drawer.hpp"

static Graphics::GLFramebuffer* create_framebuffer(int width, int height) {
    auto framebuffer_texture = new Graphics::GLTexture(Graphics::GLTextureTarget::texture_2d, Graphics::GLTextureInternalFormat::rgb32f);
    framebuffer_texture->create_texture();
    framebuffer_texture->bind();
    framebuffer_texture->set_type(Graphics::GLTextureType::type_float);
    framebuffer_texture->set_format(Graphics::GLTextureFormat::rgb);
    framebuffer_texture->set_image(width, height);
    framebuffer_texture->set_parameter(Graphics::GLTextureParameter::texture_mag_filter, GL_NEAREST);
    framebuffer_texture->set_parameter(Graphics::GLTextureParameter::texture_min_filter, GL_NEAREST);

    auto framebuffer = new Graphics::GLFramebuffer();
    framebuffer->create_framebuffer();
    framebuffer->set_texture(framebuffer_texture);
    framebuffer->bind();

    if(framebuffer->is_complete()) {
        return framebuffer;
    }

    delete framebuffer;
    delete framebuffer_texture;
    return nullptr;
}

SceneDrawer::SceneDrawer(Scene* scene, Graphics::Camera* camera, int width, int height):
        width(width), height(height), camera(camera), path_tracer_program(), accumulator_program(), renderer(), scene(scene) {
    temp_framebuffer = create_framebuffer(width, height);
    final_framebuffer_a = create_framebuffer(width, height);
    final_framebuffer_b = create_framebuffer(width, height);

    if(!temp_framebuffer || !final_framebuffer_a || !final_framebuffer_b) {
        throw SceneDrawerUnavailableException();
    }

    path_tracer_program.set_max_reflections(5);
    path_tracer_program.set_samples(1);
    path_tracer_program.set_camera(camera);

    accumulator_program.set_input_texture(temp_framebuffer->get_texture());
    accumulator_program.set_framebuffers(final_framebuffer_a, final_framebuffer_b);

    scene_buffer.create_buffers();

    frames = 0;
}
