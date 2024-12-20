#pragma once
#include "WindowSystem.h"
#include "ModelSystem.h"

// 初始化全局变量
GLFWwindow* window=nullptr;
bool showResourceExplorer = false;
bool showWindowBar = false;
std::string currentPath = std::filesystem::current_path().string();
std::vector<Window> windows = {
    {"Home", true},      // 默认一个 Home 窗口
    {"Settings", true},  // 默认一个 Settings 窗口
    {"Attribute", true}  // 默认一个 Attribute 窗口
};
int activeWindowIndex = 0;
std::vector<int> activeWindowIndices;  // 用于保存所有激活窗口的索引

// Getter 函数定义
bool getShowResourceExplorer() {
    return showResourceExplorer;
}

bool getShowWindowBar() {
    return showWindowBar;
}

std::string getCurrentPath() {
    return currentPath;
}

std::vector<Window>& getWindows() {
    return windows;
}

int getActiveWindowIndex() {
    return activeWindowIndex;
}

std::vector<int>& getActiveWindowIndices() {
    return activeWindowIndices;
}


void set_window() {
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MeshSimplification", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "[E] Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
}

