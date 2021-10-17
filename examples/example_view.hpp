#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../graphics/camera.hpp"
#include "../controls/user_controller.hpp"
#include "../scene/scene.hpp"
#include "../scene/scene_drawer.hpp"
#include "../utils/frame_timer.hpp"

class ExampleView {
protected:
    int width;
    int height;
    float scale;

    int frames_logging = 5;

    bool switch_to_dynamic_mode = true;

    Graphics::Camera* camera = nullptr;
    sf::RenderWindow* window = nullptr;
    UserController* controller = nullptr;
    Scene* scene = nullptr;
    SceneDrawer* drawer = nullptr;
    FrameTimer timer {};

    void create_window(int width, int height, float scale);
    virtual void init_window();
    virtual void init_scene();
    virtual void init_camera();
    virtual void init_drawer();

    void tick();

public:

    void init();
    void loop();

    ExampleView();
    ~ExampleView();

    void log_frame();

    void set_dynamic_mode_enabled(bool p_is_dynamic_mode);
};