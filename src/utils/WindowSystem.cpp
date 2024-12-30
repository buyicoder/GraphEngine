#pragma once
#include "WindowSystem.h"
#include "ModelSystem.h"

// 初始化全局变量
GLFWwindow* window=nullptr;
bool showResourceExplorer = true;
bool showCreateNewProjectWindow = false;
bool showWindowBar = true;
bool showModelSetting = true;
std::string currentPath = std::filesystem::current_path().string();
std::vector<Window> windows = {
};
int activeWindowIndex = 0;
std::vector<int> activeWindowIndices;  // 用于保存所有激活窗口的索引



void set_window() {
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MeshSimplification", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "[E] Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
}

