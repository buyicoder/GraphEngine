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
#include "utils/InteractSystem.h"
#include "utils/InterModeSystem.h"
#include "utils/LightSystem.h"

using Utils::Camera;
using Utils::Shader;
using Utils::Model;


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

        // render the model
        vecf3 model_pos(0.0f, 0.0f, 0.0f);
        float angle_y = base_angle_y + static_cast<float>(glfwGetTime());
        // rot matrix
        matf4 model_transform;
        model_transform <<  cos(angle_y), 0.0f, sin(angle_y), model_pos[0],
                                    0.0f, 1.0f,         0.0f, model_pos[1],
                           -sin(angle_y), 0.0f, cos(angle_y), model_pos[2],
                                    0.0f, 0.0f,         0.0f,         1.0f;
        shader.set_matf4("model", model_transform);
        meshes[current_index]->va->draw(shader);

        //render borders
        if (shows_border) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            border_shader.set_vecf3("camera_pos", camera.position);
            border_shader.set_matf4("projection", camera.get_projection_matrix(SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f));
            border_shader.set_matf4("view", camera.get_view_matrix());
            border_shader.set_matf4("model", model_transform);
            meshes[current_index]->va->draw(border_shader);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader.delete_program();

    glfwTerminate();
    return 0;
}

