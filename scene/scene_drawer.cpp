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
        width(width),
        height(height),
        camera(camera),
        tracer_program(width, height),
        accumulator_program(),
        present_program(),
        renderer(),
        scene(scene),
        frame_substeps(static_frame_substeps) {
    single_frame_buffer = create_framebuffer(width, height);
    accumulator_a = create_framebuffer(width, height);
    accumulator_b = create_framebuffer(width, height);
    result_image = create_framebuffer(width, height);

    if(!single_frame_buffer || !accumulator_a || !accumulator_b) {
        throw SceneDrawerUnavailableException();
    }

    tracer_program.set_samples(1);
    tracer_program.set_camera(camera);

    accumulator_program.set_input_texture(single_frame_buffer->get_texture());
    accumulator_program.set_framebuffers(accumulator_a, accumulator_b);

    scene_buffer.create_buffers();

    present_program.set_texture(result_image->get_texture());

    frames = 1;
    seed = time(0);
}
