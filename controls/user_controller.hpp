#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../graphics/camera.hpp"

class UserController {
    bool w_pressed = false;
    bool a_pressed = false;
    bool s_pressed = false;
    bool d_pressed = false;
    bool q_pressed = false;
    bool e_pressed = false;
    bool shift_pressed = false;
    bool space_pressed = false;
    bool up_pressed = false;
    bool right_pressed = false;
    bool left_pressed = false;
    bool down_pressed = false;
    bool p_pressed = false;

    Graphics::Camera* controlled_camera;
    sf::Window* window;

    float camera_speed = 0.1f;
public:

    explicit UserController(Graphics::Camera* camera, sf::Window* window): controlled_camera(camera), window(window) {};

    void handle_event(const sf::Event &event);
    void tick();
    bool poll_screenshot() {
        if(!p_pressed) {
            return false;
        }
        p_pressed = false;
        return true;
    }

    void set_camera_speed(float p_camera_speed);
};