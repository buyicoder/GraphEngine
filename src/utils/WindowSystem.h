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
#include "mesh_simplification.h"
#include "utils/WindowSystem.h"
#include "utils/ModelSystem.h"
#include "utils/GraphicsSystem.h"

// 窗口管理相关
struct Window {
    std::string name;
    std::string filepath;
    bool isOpen;
};

// 全局变量声明
extern GLFWwindow* window;
extern bool showResourceExplorer;
extern bool showWindowBar;
extern std::string currentPath;
extern std::vector<Window> windows;
extern int activeWindowIndex;
extern std::vector<int> activeWindowIndices;

// Getter 函数声明
bool getShowResourceExplorer();
bool getShowWindowBar();
std::string getCurrentPath();
std::vector<Window>& getWindows();
int getActiveWindowIndex();
std::vector<int>& getActiveWindowIndices();

void set_window();
// 绘制窗口栏
