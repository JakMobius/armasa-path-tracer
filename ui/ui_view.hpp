#pragma once

class UIView;

#include "../events/mouse_events.hpp"
#include "../utils/vec2.hpp"
#include "../utils/matrix3.hpp"
#include "../utils/vec4.hpp"
#include "../graphics/drawing_context.hpp"
#include <vector>

class UIView {

protected:
    Vec2f position;
    Matrix3f transform;
    Matrix3f inv_transform;
    Vec4f background;
    std::vector<UIView*> children;
    UIView* current_hovered_child = nullptr;
    UIView* current_clicked_child = nullptr;

    void transform_context(DrawingContext* ctx);
    void rehover(UIView* child, const Vec2f& internal_point);

    Vec2f size;
public:

    UIView(Vec2f position = {0, 0}, Vec2f size = {0, 0}): position(position), transform(), inv_transform(), background(1, 1, 1, 1), children(), size(size) {}
    virtual ~UIView() {};

    virtual void draw(DrawingContext* ctx);
    virtual void on_mouse_in(MouseInEvent *event);
    virtual void on_mouse_move(MouseMoveEvent *event);
    virtual void on_mouse_out(MouseOutEvent *event);
    virtual void on_mouse_down(MouseDownEvent *event);
    virtual void on_mouse_up(MouseUpEvent *event);

    const Vec2f& get_position() { return position; }
    virtual void set_position(const Vec2f& pos) { position = pos; }

    const Vec2f& get_size() { return size; }
    virtual void set_size(const Vec2f& new_size) { size = new_size; }

    const Vec4f& get_background() { return background; }
    virtual void set_background(const Vec4f& new_background) { background = new_background; }

    const Matrix3f& get_transform() { return transform; }
    virtual void set_transform(const Matrix3f& new_transform) { transform = new_transform; inv_transform = transform.inverse(); }

    Vec2f get_local_position(const Vec2f& external_position);

    void append_child(UIView* child) { children.push_back(child); }

    const std::vector<UIView*>& get_children() const { return children; }

    const Matrix3f& get_inv_transform() { return inv_transform; };

    UIView* test(const Vec2f &point, Vec2f* internal_point) const;
};