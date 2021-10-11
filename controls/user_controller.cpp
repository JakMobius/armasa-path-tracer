//
// Created by Артем on 08.09.2021.
//

#include "user_controller.hpp"

void UserController::handle_event(const sf::Event &event)  {
    if (event.type == sf::Event::Closed) window->close();
    else if(event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
            case sf::Keyboard::W: w_pressed = true; break;
            case sf::Keyboard::A: a_pressed = true; break;
            case sf::Keyboard::S: s_pressed = true; break;
            case sf::Keyboard::D: d_pressed = true; break;
            case sf::Keyboard::Q: q_pressed = true; break;
            case sf::Keyboard::E: e_pressed = true; break;
            case sf::Keyboard::LShift: shift_pressed = true; break;
            case sf::Keyboard::Space: space_pressed = true; break;
            case sf::Keyboard::Up: up_pressed = true; break;
            case sf::Keyboard::Left: left_pressed = true; break;
            case sf::Keyboard::Down: down_pressed = true; break;
            case sf::Keyboard::Right: right_pressed = true; break;
            case sf::Keyboard::P: p_pressed = true; break;
            default: break;
        }
    } else if(event.type == sf::Event::KeyReleased) {
        switch(event.key.code) {
            case sf::Keyboard::W: w_pressed = false; break;
            case sf::Keyboard::A: a_pressed = false; break;
            case sf::Keyboard::S: s_pressed = false; break;
            case sf::Keyboard::D: d_pressed = false; break;
            case sf::Keyboard::Q: q_pressed = false; break;
            case sf::Keyboard::E: e_pressed = false; break;
            case sf::Keyboard::LShift: shift_pressed = false; break;
            case sf::Keyboard::Space: space_pressed = false; break;
            case sf::Keyboard::Up: up_pressed = false; break;
            case sf::Keyboard::Left: left_pressed = false; break;
            case sf::Keyboard::Down: down_pressed = false; break;
            case sf::Keyboard::Right: right_pressed = false; break;
            //case sf::Keyboard::P: p_pressed = false; break;
            default: break;
        }
    }
}

void UserController::tick() {

    Vec3f movement = {
        (float) ((w_pressed ? 1.f : 0.f) + (s_pressed ? -1.f : 0.f)) * camera_speed,
        (float)((d_pressed ? 1.f : 0.f) + (a_pressed ? -1.f : 0.f)) * camera_speed,
        (float)((shift_pressed ? -1.f : 0.f) + (space_pressed ? 1.f : 0.f)) * camera_speed
    };

    float camera_move_y = (float)((up_pressed ? 1.f : 0.f) + (down_pressed ? -1.f : 0.f)) * 0.01f;
    float camera_move_x = (float)((right_pressed ? 1.f : 0.f) + (left_pressed ? -1.f : 0.f)) * 0.01f;
    float camera_move_yaw = (float)((q_pressed ? 1.f : 0.f) + (e_pressed ? -1.f : 0.f)) * 0.01f;

    if(movement.is_zero() && camera_move_x == 0 && camera_move_y == 0 && camera_move_yaw == 0) return;
    movement.transform_unbound(controlled_camera->matrix);

    Matrix4f& new_matrix = controlled_camera->matrix;

    auto position = controlled_camera->get_position();
    controlled_camera->set_position(position + movement);

    new_matrix = Matrix4f::rotation_x_matrix(camera_move_yaw) * new_matrix;
    new_matrix = Matrix4f::rotation_y_matrix(camera_move_y) * new_matrix;
    new_matrix = Matrix4f::rotation_z_matrix(camera_move_x) * new_matrix;

    controlled_camera->matrix = new_matrix;
    controlled_camera->set_moved();

}

void UserController::set_camera_speed(float p_camera_speed) {
    camera_speed = p_camera_speed;
}
