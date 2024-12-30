#include "GraphicsSystem.h"
#include <imgui.h>

// ͼ��ϵͳ��������������
void loadChineseFont() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../fonts/Times New Roman.ttf", 12.0f);
}

// ͼ��ϵͳ����Ļ����
uint32_t SCR_WIDTH = 1600;
uint32_t SCR_HEIGHT = 1200;

// Getter ����ʵ��
uint32_t getScreenWidth() {
    return SCR_WIDTH;
}

uint32_t getScreenHeight() {
    return SCR_HEIGHT;
}