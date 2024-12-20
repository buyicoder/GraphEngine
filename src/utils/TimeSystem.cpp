#include "TimeSystem.h"

float delta_time = 0.0f;  // time between current frame and last frame
float last_frame = 0.0f;  // last frame time

// ʱ��ϵͳ����ȡ����ʵ��
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