#include "ui_view.hpp"

void UIView::transform_context(DrawingContext* ctx) {
    ctx->transform.translate(position[0], position[1]);
    ctx->transform.multiply(transform);
}

void UIView::draw(DrawingContext* ctx) {

    if(background[3] > 0) {
        ctx->color = background.to_sf_color();
        ctx->draw_rect({0, 0}, size);
    }

    for(int i = 0; i < children.size(); i++) {
        auto* child = children[i];
        Matrix3f saved_transform = ctx->transform;
        child->transform_context(ctx);
        child->draw(ctx);
        ctx->transform = saved_transform;
    }
}

void UIView::on_mouse_in(MouseInEvent* event) {
    Vec2f internal_point;
    UIView* child = test(Vec2f { event->x, event->y }, &internal_point);

    if(child) {
        MouseInEvent nested_event(internal_point[0], internal_point[1]);

        child->on_mouse_in(&nested_event);
        current_hovered_child = child;
    }
}

UIView* UIView::test(const Vec2f& point, Vec2f* internal_point) const {
    for(int i = get_children().size() - 1; i >= 0; i--) {
        UIView* child = get_children()[i];
        Vec2f absolute_point = point;
        absolute_point -= child->get_position();
        absolute_point *= child->get_inv_transform();
        if(absolute_point[0] < 0 || absolute_point[0] >= child->get_size()[0]) continue;
        if(absolute_point[1] < 0 || absolute_point[1] >= child->get_size()[1]) continue;
        *internal_point = absolute_point;
        return child;
    }
    return nullptr;
}

void UIView::on_mouse_move(MouseMoveEvent* event) {
    Vec2f internal_point;
    UIView* child = nullptr;

    if(current_clicked_child) {
        child = current_clicked_child;
        internal_point = child->get_local_position(Vec2f { event->x, event->y });
    } else {
        child = test(Vec2f {event->x, event->y}, &internal_point);
    }

    if(child && child == current_hovered_child) {
        double dx = child->transform.transform_x(event->dx, event->dy, 0);
        double dy = child->transform.transform_y(event->dx, event->dy, 0);
        MouseMoveEvent nested_event(internal_point[0], internal_point[1], dx, dy);
        child->on_mouse_move(&nested_event);
    } else rehover(child, internal_point);
}

void UIView::on_mouse_out(MouseOutEvent* event) {

    if(current_hovered_child) {
        Vec2f internal_point = Vec2f(event->x, event->y);
        MouseOutEvent nested_event(internal_point[0], internal_point[1]);
        current_hovered_child->on_mouse_out(&nested_event);
        current_hovered_child = nullptr;
    }

    if(current_clicked_child) {
        Vec2f internal_point = Vec2f(event->x, event->y);
        MouseUpEvent nested_event(internal_point[0], internal_point[1]);
        current_clicked_child->on_mouse_up(&nested_event);
        current_clicked_child = nullptr;
    }
}

void UIView::on_mouse_down(MouseDownEvent* event) {
    current_clicked_child = current_hovered_child;
    if(current_clicked_child) {
        Vec2f internal_point = current_clicked_child->get_local_position({ event->x, event->y });
        MouseDownEvent nested_event(internal_point[0], internal_point[1]);

        current_clicked_child->on_mouse_down(&nested_event);
    }
}

void UIView::on_mouse_up(MouseUpEvent* event) {
    if(current_clicked_child) {
        Vec2f internal_point = current_clicked_child->get_local_position({ event->x, event->y });
        MouseUpEvent nested_event(internal_point[0], internal_point[1]);

        current_clicked_child->on_mouse_up(&nested_event);

        UIView* child = test(Vec2f {event->x, event->y}, &internal_point);
        if(child != current_hovered_child) rehover(child, internal_point);
        current_clicked_child = nullptr;
    }
}

Vec2f UIView::get_local_position(const Vec2f &external_position) {
    Vec2f internal_point = external_position;
    internal_point -= position;
    internal_point *= get_inv_transform();
    return internal_point;
}

void UIView::rehover(UIView* child, const Vec2f& internal_point) {
    if(child) {
        MouseInEvent nested_mouse_in_event(internal_point[0], internal_point[1]);
        child->on_mouse_in(&nested_mouse_in_event);
    }
    if(current_hovered_child) {
        MouseOutEvent nested_mouse_out_event(internal_point[0], internal_point[1]);
        current_hovered_child->on_mouse_out(&nested_mouse_out_event);
    }
    current_hovered_child = child;
}
