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
#include <vector>

// 变量声明和 getter 函数声明
extern std::vector<int> vertexCountIndices;
extern std::vector<std::unique_ptr<Model>> meshes;
extern int current_index;
extern float simplification_ratio;
extern bool shows_border;
extern bool isInsideVertexCountsWindow;
extern bool isButtonClicked;

// Getter 函数声明
std::vector<int>* getVertexCountIndices();
std::vector<std::unique_ptr<Model>>& getMeshes();
int* getCurrentIndex();
float* getSimplificationRatio();
bool* getShowsBorder();
bool* getIsInsideVertexCountsWindow();
bool* getIsButtonClicked();
void load_meshes();