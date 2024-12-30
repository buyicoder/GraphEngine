#pragma once
#include "WindowSystem.h"
#include "ModelSystem.h"

// ��ʼ��ȫ�ֱ���
GLFWwindow* window=nullptr;
bool showResourceExplorer = true;
bool showCreateNewProjectWindow = false;
bool showWindowBar = true;
bool showModelSetting = true;
std::string currentPath = std::filesystem::current_path().string();
std::vector<Window> windows = {
};
int activeWindowIndex = 0;
std::vector<int> activeWindowIndices;  // ���ڱ������м���ڵ�����



void set_window() {
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MeshSimplification", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "[E] Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
}

