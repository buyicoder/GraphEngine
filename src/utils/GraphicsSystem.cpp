#include "GraphicsSystem.h"
#include <imgui.h>

// 图形系统：加载中文字体
void loadChineseFont() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../fonts/Times New Roman.ttf", 12.0f);
}

// 图形系统：屏幕设置
uint32_t SCR_WIDTH = 1600;
uint32_t SCR_HEIGHT = 1200;

// Getter 函数实现
uint32_t getScreenWidth() {
    return SCR_WIDTH;
}

uint32_t getScreenHeight() {
    return SCR_HEIGHT;
}