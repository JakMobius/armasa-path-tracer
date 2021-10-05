
#include <random>
#include "graphics/window.hpp"
#include "graphics/programs/path_tracer_program.hpp"
#include "controls/user_controller.hpp"
#include "scene/scene.hpp"
#include "scene/hittables/hittable_sphere.hpp"
#include "scene/hittables/hittable_list.hpp"
#include "scene/cornell_box_scene.hpp"
#include "graphics/gl/gl_framebuffer.hpp"
#include "graphics/programs/accumulator_program.hpp"
#include "graphics/programs/present_program.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj/tiny_obj_loader.h"
#include "scene/hittables/hittable_triangle.hpp"
#include "scene/materials/material_lambertian.hpp"

Graphics::GLFramebuffer* create_framebuffer(int width, int height) {
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

void app() {
    const int width = 500;
    const int height = 500;

    Window window(width, height, 1);
    glViewport(0, 0, width, height);

    Graphics::PathTracerProgram path_tracer_program;
    Graphics::AccumulatorProgram accumulator_program;
    Graphics::PresentProgram present_program;
    Graphics::Camera camera {};
    UserController controller(&camera, &window, nullptr);

    Graphics::GLFramebuffer* temp_framebuffer = create_framebuffer(width, height);
    Graphics::GLFramebuffer* final_framebuffer_a = create_framebuffer(width, height);
    Graphics::GLFramebuffer* final_framebuffer_b = create_framebuffer(width, height);

    if(!temp_framebuffer || !final_framebuffer_a || !final_framebuffer_b) return;

    Scene* scene = cornell_box_scene();
    SceneBuffer scene_buffer;
    SceneRenderer renderer(scene);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::string err;

    if(!tinyobj::LoadObj(&attrib, &shapes, nullptr, &err, "resources/models/ship.obj", nullptr)) {
        std::cout << err;
        return;
    }

    Material *m_red   = new MaterialLambertian({1, 223.0/255.0, 196.0/255.0});

    Matrix4f m = Matrix4f::rotation_x_matrix(M_PI / 2) *
            Matrix4f::rotation_z_matrix(M_PI / 2) *
            Matrix4f::scale_matrix(0.5, 0.5, 0.5) *
            Matrix4f::translation_matrix(1, 0, 0);

    for(auto& shape : shapes) {
        for(int i = 0; i < shape.mesh.indices.size(); i += 3) {
            int index_a = shape.mesh.indices[i].vertex_index;
            int index_b = shape.mesh.indices[i + 1].vertex_index;
            int index_c = shape.mesh.indices[i + 2].vertex_index;

            Vec3f point_a { attrib.vertices[index_a * 3 + 0], attrib.vertices[index_a * 3 + 1], attrib.vertices[index_a * 3 + 2] };
            Vec3f point_b { attrib.vertices[index_b * 3 + 0], attrib.vertices[index_b * 3 + 1], attrib.vertices[index_b * 3 + 2] };
            Vec3f point_c { attrib.vertices[index_c * 3 + 0], attrib.vertices[index_c * 3 + 1], attrib.vertices[index_c * 3 + 2] };

            point_a *= m;
            point_b *= m;
            point_c *= m;

            scene->get_root_hittable()->add_children(new HittableTriangle(point_a, point_b, point_c, m_red));
        }
    }

    scene_buffer.create_buffers();

    camera.set_position({-15, 0, 0});
    camera.set_focus_distance(4.5);
    camera.set_camera_width((float)window.get_width() / (float)window.get_height());

    path_tracer_program.set_max_reflections(3);
    path_tracer_program.set_samples(1);
    path_tracer_program.set_camera(&camera);

    accumulator_program.set_input_texture(temp_framebuffer->get_texture());
    accumulator_program.set_framebuffers(final_framebuffer_a, final_framebuffer_b);

    present_program.set_brightness(3.0);
    present_program.set_gamma(0.4);

    sf::Event event {};

    int frames = 0;
    int measured_frames = 5;
    unsigned long frame_microseconds = 0;

    while(true) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        while (window.get_sf_window()->pollEvent(event)) {
            controller.handle_event(event);
        }

        if(!window.is_open()) break;

        controller.tick();

        bool clear = camera.is_moved();
        if(clear) {
            scene->get_renderer()->render(&scene_buffer);
            frames = 0;
        }

        frames++;

        window.clear();
        temp_framebuffer->bind();
        path_tracer_program.draw(&scene_buffer);
        accumulator_program.draw(clear);
        present_program.set_texture(accumulator_program.get_last_target_texture());
        Graphics::GLFramebuffer::unbind();
        present_program.draw(frames);
        window.swap();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        frame_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        if(frames % measured_frames == 0) {
            double rays_per_second = (double) measured_frames * (double) path_tracer_program.get_samples() / (double)frame_microseconds * 1000000 * (double)(width * height);
            std::cout << "completed frame " << frames << ", "
                    << (long long)rays_per_second << " rays per second, "
                    << (long long)frames * (long long)width * (long long)height << " rays in total\n";

            frame_microseconds = 0;
        }
    }
}
int main() {
//    app();
    try {
        app();
    } catch(GLException &ex) {
        std::cout << "GLException: " << ex.what() << "\n";
    } catch(GLStringException &ex) {
        std::cout << "GLStringException: " << ex.what() << "\n";
    }
}