#pragma once

struct MouseEvent {
    float x, y;

    MouseEvent(float x, float y): x(x), y(y) {};
};

struct MouseDownEvent : MouseEvent {
    MouseDownEvent(float x, float y): MouseEvent(x, y) {};
};

struct MouseUpEvent : MouseEvent {
    MouseUpEvent(float x, float y): MouseEvent(x, y) {};
};

struct MouseMoveEvent : MouseEvent {
    float dx, dy;

    MouseMoveEvent(float x, float y, float dx, float dy): MouseEvent(x, y), dx(dx), dy(dy) {}
};
struct MouseOutEvent : MouseEvent {
    MouseOutEvent(float x, float y): MouseEvent(x, y) {}
};
struct MouseInEvent : MouseEvent {
    MouseInEvent(float x, float y): MouseEvent(x, y) {}
};