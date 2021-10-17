//
// Created by Артем on 11.10.2021.
//

#include <SFML/Window/ContextSettings.hpp>
#include <GL/glew.h>
#include "example_view.hpp"

ExampleView::ExampleView() {

}

void ExampleView::create_window(int p_width, int p_height, float p_scale) {
    width = p_width;
    height = p_height;
    scale = p_scale;

    sf::ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 1;

    window = new sf::RenderWindow(sf::VideoMode(width * scale, height * scale, 32), "Tracer",
                                  sf::Style::Titlebar | sf::Style::Close, settings);
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);
    window->setActive(true);
}

void ExampleView::init_window() {
    create_window(1000, 1000, 1);
}

void ExampleView::init_scene() {
    scene = new Scene();
}

void ExampleView::init_camera() {
    camera = new Graphics::Camera();
    controller = new UserController(camera, window);

    camera->set_camera_width((float)width / (float)height);
}

void ExampleView::init_drawer() {
    drawer = new SceneDrawer(scene, camera, width, height);
}

ExampleView::~ExampleView() {
    delete window;
    delete scene;
    delete camera;
    delete drawer;
    delete controller;
}

void ExampleView::init() {
    init_window();
    glewInit();
    init_scene();
    init_camera();
    init_drawer();
}

void ExampleView::loop() {
    sf::Event event {};

    while(true) {
        while (window->pollEvent(event)) controller->handle_event(event);
        if(!window->isOpen()) break;

        tick();
    }
}

void ExampleView::set_dynamic_mode_enabled(bool p_is_dynamic_mode) {
    if(p_is_dynamic_mode == drawer->get_dynamic_mode()) return;

    drawer->set_dynamic_mode(p_is_dynamic_mode);
}

void ExampleView::tick() {

    controller->tick();

    if(controller->poll_screenshot()) {
        drawer->take_screenshot();
        std::cout << "Screenshot queried\n";
    }

    if(switch_to_dynamic_mode) set_dynamic_mode_enabled(camera->is_moved());
    else if(camera->is_moved()) drawer->reset();

    timer.begin_frame();
    drawer->draw_chunk();

//    if(drawer->get_frame_substep() == 0) {
        glViewport(0, 0, width * scale, height * scale);
        drawer->present();
        window->display();
//    }

    timer.end_frame();

    log_frame();
}

void ExampleView::log_frame() {
    if(drawer->get_frames() % frames_logging == 0 && drawer->get_frame_substep() == 0) {
        double rays_per_second = (double) frames_logging * (double) drawer->get_frame_rays() / timer.flush();

        std::cout << "completed frame " << drawer->get_frames() << ", ";
        std::cout << (long long) rays_per_second << " rays per second, ";
        std::cout << (long long) drawer->get_frames() * (long long) drawer->get_frame_rays() << " rays in total\n";
    }
}
