#pragma once
#include "WindowSystem.h"
#include "ModelSystem.h"

// ��ʼ��ȫ�ֱ���
GLFWwindow* window=nullptr;
bool showResourceExplorer = false;
bool showWindowBar = false;
std::string currentPath = std::filesystem::current_path().string();
std::vector<Window> windows = {
    {"Home", true},      // Ĭ��һ�� Home ����
    {"Settings", true},  // Ĭ��һ�� Settings ����
    {"Attribute", true}  // Ĭ��һ�� Attribute ����
};
int activeWindowIndex = 0;
std::vector<int> activeWindowIndices;  // ���ڱ������м���ڵ�����

// Getter ��������
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

