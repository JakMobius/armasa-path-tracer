#pragma once

#include <chrono>

class FrameTimer {

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    long long frame_microseconds = 0;

public:
    FrameTimer() {};

    void begin_frame() { begin = std::chrono::steady_clock::now(); }
    void end_frame() {
        end = std::chrono::steady_clock::now();
        frame_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    }

    double flush() {
        double result = (double) frame_microseconds / 1000000;
        frame_microseconds = 0;
        return result;
    }
};