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
#include "utils/CameraSystem.h"
#include "utils/TimeSystem.h"
#include "utils/InterModeSystem.h"
using Utils::Camera;
using Utils::Shader;
using Utils::Model;

// 输入系统：鼠标回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int code, const char* description);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void set_callbacks();
//鼠标是否点击了场景内
bool DetectMouseOutsideClick();

// 输入系统：鼠标和键盘状态
extern bool is_left_pressing;
extern bool is_right_pressing;
extern bool is_b_pressing;
extern bool is_changing;


