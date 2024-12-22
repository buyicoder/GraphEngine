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
extern int InterMode;
enum InterMode {
	SCENEEXCURISION,//场景游览模式
	GLOBALEDIT,//全局编辑模式
	TEXTEDIT//文字编辑模式
};
void SetInterMode();