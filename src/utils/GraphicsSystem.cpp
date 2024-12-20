#include "GraphicsSystem.h"
#include <imgui.h>

// ͼ��ϵͳ��������������
void loadChineseFont() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../fonts/Times New Roman.ttf", 12.0f);
}

// ͼ��ϵͳ����Ļ����
uint32_t SCR_WIDTH = 800;
uint32_t SCR_HEIGHT = 600;

// Getter ����ʵ��
uint32_t getScreenWidth() {
    return SCR_WIDTH;
}

uint32_t getScreenHeight() {
    return SCR_HEIGHT;
}