#include <iostream>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> // 提供常用矩阵变换函数，例如投影矩阵、旋转矩阵等
#include <glm/gtc/type_ptr.hpp>   
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
#include "utils/InteractSystem.h"
#include "utils/InterModeSystem.h"
#include "utils/LightSystem.h"
#include "utils/SceneManageSystem.h"

using Utils::Camera;
using Utils::Shader;
using Utils::Model;

float axisVertices[] = {
    // Positions         // Colors
    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,  // X-axis (red)
    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,  // Y-axis (green)
    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,  // Z-axis (blue)
    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
};

unsigned int axisVAO, axisVBO;

void InitAxis() {
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // 设置顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 设置颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DrawAxis(Shader& axisShader) {
    // 保存当前视口
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // 设置新的视口（左下角 100x100 区域）
    glViewport(10, 10, 100, 100);

    // 使用坐标轴着色器
    axisShader.use_program();

    // 设置投影和视图矩阵
    matf4 projection = matf4::Identity(); // 正交投影
    matf4 view = matf4::Identity(); // 单位矩阵，固定视图
    matf4 model = matf4::Identity(); // 单位矩阵，不做变换

    axisShader.set_matf4("projection", projection);
    axisShader.set_matf4("view", view);
    axisShader.set_matf4("model", model);

    // 绘制坐标轴
    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);

    // 恢复原视口
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

int main(int argc, char **argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // set window and context
    set_window();

    // set callbacks
    set_callbacks();
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[E] Failed to initialize GLAD." << std::endl;
        return -2;
    }

    // set for imgui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    static_cast<void>(io);
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

        // 加载支持中文的字体
    loadChineseFont();


    // shader setting
    Shader shader(SHADER_DIR"/p3n3.vert", SHADER_DIR"/light.frag");
    Shader border_shader(SHADER_DIR"/p3n3.vert", SHADER_DIR"/border.frag");



    shader.set_vecf3("point_light_pos", point_light_pos);
    shader.set_vecf3("point_light_radiance", point_light_radiance);
    shader.set_vecf3("local_color", {1.0f, 1.0f, 1.0f});
    shader.set_vecf3("ambient_irradiance", ambient);
    shader.set_float("roughness", 0.6f);
    shader.set_float("metalness", 0.1f);

    vecf3 border_color(1.0f, 1.0f, 0.0f);

    border_shader.set_vecf3("border_color", border_color);
    
    // load meshes
    load_meshes();
    InitFiles();
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);


    unsigned int VBO, VAO;
    std::vector<float> gridVertices = GenerateGridVertices(20.0f, 1.0f); // 10x10 网格，间隔 1

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 初始化 Shader 和坐标轴
    Shader axisShader("axis.vert", "axis.frag");
    InitAxis();

    while (!glfwWindowShouldClose(window)) {
        // record time
        RecordTime();
        
        SetInterMode();
        // while calculating the meshes and vertices, I recommend avoid processing input
        if (!is_changing) {
            process_input(window);
        } else {
             if (*getCurrentIndex() < getMeshes().size() - 1) {
                 *getCurrentIndex() += 1;
             } else {
                 is_changing = false;
             }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawEditor();
        
        glClearColor(ambient[0], ambient[1], ambient[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL);

        shader.set_vecf3("camera_pos", camera.position);
        shader.set_matf4("projection", camera.get_projection_matrix(SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f));
        shader.set_matf4("view", camera.get_view_matrix());


        shader.set_matf4("model", matf4::Identity());
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, gridVertices.size() / 3); // 每 3 个顶点为一组
        glBindVertexArray(0);

        DrawModel(shader, border_shader);
       
        // 绘制坐标轴
        shader.set_matf4("model", matf4::Identity());
        DrawAxis(axisShader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    shader.delete_program();

    glfwTerminate();
    return 0;
}

