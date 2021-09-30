#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../graphics/programs/camera_uniform_controller/camera_uniform_controller.hpp"
#include "../ui/ui_view.hpp"
#include "../graphics/window.hpp"

class UserController {
    bool w_pressed = false;
    bool a_pressed = false;
    bool s_pressed = false;
    bool d_pressed = false;
    bool shift_pressed = false;
    bool space_pressed = false;
    bool up_pressed = false;
    bool right_pressed = false;
    bool left_pressed = false;
    bool down_pressed = false;
    Graphics::Camera* controlled_camera;
    Window* window;
    UIView* root_view;

    int old_mouse_x = -1;
    int old_mouse_y = -1;
public:

    explicit UserController(Graphics::Camera* camera, Window* window, UIView* root_view): controlled_camera(camera), window(window), root_view(root_view) {};

    void handle_event(const sf::Event &event);
    void tick();

};