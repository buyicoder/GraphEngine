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
#include <utils/LightSystem.h>
using Utils::Shader;