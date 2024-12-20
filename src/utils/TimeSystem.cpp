#include "TimeSystem.h"

float delta_time = 0.0f;  // time between current frame and last frame
float last_frame = 0.0f;  // last frame time

// 时间系统：获取函数实现
float getDeltaTime() {
    return delta_time;
}

float getLastFrame() {
    return last_frame;
}

void RecordTime() {
    auto current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
}