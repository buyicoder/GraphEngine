#include "CameraSystem.h"

Camera camera(vecf3(0.0f, 0.0f, 3.0f));
Camera forwardCamera(vecf3(0.0f, 0.0f, 3.0f));
Camera backCamera(vecf3(0.0f, 0.0f, -3.0f));
Camera leftCamera(vecf3(3.0f, 0.0f, 0.0f));
Camera rightCamera(vecf3(-3.0f, 0.0f, 0.0f));
Camera upCamera(vecf3(0.0f, 3.0f, 0.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool init_mouse = true;
bool camera_movement_enabled = true;
float base_angle_y = 0.0f;
float angle_x = 0.0f;

// 摄像机系统：获取函数实现
Camera* getCamera() {
    return &camera;
}

float getLastX() {
    return last_x;
}

float getLastY() {
    return last_y;
}

bool getInitMouse() {
    return init_mouse;
}

bool getCameraMovementEnabled() {
    return camera_movement_enabled;
}

float getBaseAngleY() {
    return base_angle_y;
}

float getAngleX() {
    return angle_x;
}
