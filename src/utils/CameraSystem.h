#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Eigen/Dense"

#include "utils/shader.h"
#include "utils/camera.h"
#include "utils/model.h"
#include "utils/tools.h"
#include "utils/WindowSystem.h"
#include "utils/ModelSystem.h"
#include "utils/GraphicsSystem.h"

using Utils::Camera;
using Utils::Shader;
using Utils::Model;


extern Camera camera;
extern float last_x;
extern float last_y;
extern bool init_mouse;
extern bool camera_movement_enabled;
extern float base_angle_y;
extern float angle_x;
extern int currentCamera;
enum camera {
    MAINCAMERA,
    FORWARDCAMERA,
    BACKCAMERA,
    LEFTCAMERA,
    RIGHTCAMERA,
    UPCAMERA
};
// 摄像机系统：获取函数声明
Camera *getCamera();
float getLastX();
float getLastY();
bool getInitMouse();
bool getCameraMovementEnabled();
float getBaseAngleY();
float getAngleX();