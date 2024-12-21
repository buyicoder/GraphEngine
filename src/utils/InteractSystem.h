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
#include "utils/InputSystem.h"
#include "utils/CameraSystem.h"
#include "utils/FileSystem.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>

using Utils::Camera;
using Utils::Shader;
using Utils::Model;

void DrawWindowBar();
// 绘制主内容区域
void DrawMainContent(std::vector<std::unique_ptr<Model>>& meshes);
// 主菜单栏
void DrawMenuBar();
bool DrawHoverBorder(const char* label, const char* shortcut = nullptr);
void SetupImGuiStyle();
void DrawResourceExplorer();
void DisplayDirectoryContents(const std::string& path);
void DrawMainMenu();
void DrawEditor();
void DisplayFileContents(const std::string& filepath);