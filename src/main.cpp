#include <iostream>
#include <cstdint>
#include <vector>

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

using Utils::Camera;
using Utils::Shader;
using Utils::Model;

// 新增：加载支持中文的字体
void loadChineseFont() {
    ImGuiIO& io = ImGui::GetIO();
    // 加载字体文件，这里假设字体文件名为 "fonts/NotoSansSC-Regular.otf"
    io.Fonts->AddFontFromFileTTF("../../../fonts/Times New Roman.ttf", 12.0f);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int code, const char *description);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen settings
static uint32_t SCR_WIDTH = 800;
static uint32_t SCR_HEIGHT = 600;

// camera
Camera camera(vecf3(0.0f, 0.0f, 3.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool init_mouse = true;
bool camera_movement_enabled = true;
float base_angle_y = 0.0f;
float angle_x = 0.0f;

// time
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

// runtime status
bool is_left_pressing = false;
bool is_right_pressing = false;
bool is_changing = false;
bool is_b_pressing = false;
bool shows_border = false;

bool isInsideVertexCountsWindow = false;
bool isButtonClicked = false;
// 新增：存储不同顶点数目的模型的索引
std::vector<int> vertexCountIndices;



// model simplification
std::vector<std::unique_ptr<Model>> meshes;
int current_index = 0;
float simplification_ratio = 0.99f;

int main(int argc, char **argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // set window and context
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MeshSimplification", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "[E] Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
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

    vecf3 ambient(0.2f, 0.2f, 0.2f);
    vecf3 point_light_pos(0.0f, 10.0f, 0.0f);
    vecf3 point_light_radiance(255.0f, 255.0f, 255.0f);

    shader.set_vecf3("point_light_pos", point_light_pos);
    shader.set_vecf3("point_light_radiance", point_light_radiance);
    shader.set_vecf3("local_color", {1.0f, 1.0f, 1.0f});
    shader.set_vecf3("ambient_irradiance", ambient);
    shader.set_float("roughness", 0.6f);
    shader.set_float("metalness", 0.1f);

    vecf3 border_color(1.0f, 1.0f, 0.0f);

    border_shader.set_vecf3("border_color", border_color);
    
    // load meshes
    meshes.emplace_back(Model::load(RESOURCES_DIR"/squirrel.obj"));
    vertexCountIndices.push_back(0);


    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // record time
        auto current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // while calculating the meshes and vertices, I recommend avoid processing input
        if (!is_changing) {
            process_input(window);
        } else {
             if (current_index < meshes.size() - 1) {
                 current_index += 1;
             } else {
                 is_changing = false;
             }
        }

        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 增大字体缩放因子
        ImGui::GetIO().FontGlobalScale = 3.0f; // 这里将字体放大 1.5 倍，你可以根据需要调整这个值

        // 获取屏幕尺寸
        ImVec2 screen_size = ImGui::GetIO().DisplaySize;
        // 设置窗口大小为屏幕尺寸的 25%
        ImVec2 window_size = ImVec2(screen_size.x * 0.25f, screen_size.y * 1.0f);



        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Always);

        
        //ImGui::Begin("attribute", nullptr,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar);

        //// 创建一个可拖动的子窗口
        //ImVec2 childWindowSize = ImVec2(screen_size.x, screen_size.y * 0.8f);
        //ImGui::BeginChild("VertexCounts", childWindowSize, true, ImGuiWindowFlags_AlwaysAutoResize);


        //ImGui::Text("vertices: %d", meshes[current_index]->positions.size());
        //ImGui::Text("faces: %d", meshes[current_index]->indices.size());
        //ImGui::Text("borders: %s", shows_border ? "On" : "Off");

        //static int counter = 0;
        //static float f = 0.0f;

        //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //    counter++;
        //ImGui::SameLine();
        //ImGui::Text("counter = %d", counter);

        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);


        //for (size_t i = 0; i < vertexCountIndices.size(); ++i) {
        //    std::string buttonLabel = std::to_string(meshes[vertexCountIndices[i]]->positions.size());
        //    if (ImGui::TreeNodeEx(buttonLabel.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth)) {
        //        // 当按钮被点击时，切换到相应顶点数的模型
        //        current_index = i;
        //        isButtonClicked = true;

        //        // 在树节点展开时显示额外的信息
        //    ImGui::Text("Additional Information:");
        //    ImGui::Text("Vertices: %d", meshes[vertexCountIndices[i]]->positions.size());
        //    ImGui::Text("Faces: %d", meshes[vertexCountIndices[i]]->indices.size());

        //        ImGui::TreePop(); // 关闭树节点
        //    }
        //    // 检查鼠标是否在当前按钮上
        //    if (ImGui::IsItemClicked(0)) {
        //        isInsideVertexCountsWindow = true;
        //        std::cout << buttonLabel.c_str() << " was clicked" << std::endl;
        //        current_index = i;
        //        isButtonClicked = true;
        //    }
        //    //std::cout << "current_index has been " << vertexCountIndices[i] << std::endl;
        //    // 在每个 TreeNodeEx 之后添加 SameLine，实现横向排列
        //    if (i < vertexCountIndices.size() - 1) {
        //    ImGui::SameLine();
        //    }
        //}


        //ImGui::EndChild(); // 结束子窗口    
        //ImGui::End();

        ImGui::Begin("topOption", nullptr,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar);
            ImGui::InvisibleButton("##Files", ImVec2(30, 30)); // 设置点击区域的大小为 30x30 像素
            if (ImGui::TreeNodeEx("Files", ImGuiTreeNodeFlags_OpenOnArrow)) {

            ImGui::Text("Show Files Options");

                ImGui::TreePop(); // 关闭树节点
            }
            ////实现横向排列
            //ImGui::SameLine();

            if (ImGui::TreeNodeEx("Editor", ImGuiTreeNodeFlags_OpenOnArrow)) {

                ImGui::Text("Show Files Options");

                ImGui::TreePop(); // 关闭树节点
            }
            ////实现横向排列
            //ImGui::SameLine();

            if (ImGui::TreeNodeEx("View", ImGuiTreeNodeFlags_OpenOnArrow)) {

                ImGui::Text("Show Files Options");

                ImGui::TreePop(); // 关闭树节点
            }
            ////实现横向排列
            //ImGui::SameLine();

            if (ImGui::TreeNodeEx("Git", ImGuiTreeNodeFlags_OpenOnArrow)) {

                ImGui::Text("Show Files Options");

                ImGui::TreePop(); // 关闭树节点
            }
            ////实现横向排列
            //ImGui::SameLine();

            if (ImGui::TreeNodeEx("Projects", ImGuiTreeNodeFlags_OpenOnArrow)) {

                ImGui::Text("Show Files Options");

                ImGui::TreePop(); // 关闭树节点
            }
            ////实现横向排列
            //ImGui::SameLine();

            if (ImGui::TreeNodeEx("Build", ImGuiTreeNodeFlags_OpenOnArrow)) {

                ImGui::Text("Show Files Options");

                ImGui::TreePop(); // 关闭树节点
            }
            ////实现横向排列
            //ImGui::SameLine();


        ImGui::End();

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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void error_callback(int code, const char *description) {
    std::cerr << "[E]" << code << " : " << description << std::endl;
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        camera_movement_enabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.process_keyboard(Camera::Movement::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.process_keyboard(Camera::Movement::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.process_keyboard(Camera::Movement::LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.process_keyboard(Camera::Movement::RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.process_keyboard(Camera::Movement::UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { 
        camera.process_keyboard(Camera::Movement::DOWN, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        is_b_pressing = true;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        is_left_pressing = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        is_right_pressing = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        is_changing = true;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && is_b_pressing) {
        is_b_pressing = false;
        shows_border = !shows_border;
    }
   

    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) && is_left_pressing) {
        if (current_index > 0) {
            current_index -= 1;
        }
        is_left_pressing = false;
    }
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) && is_right_pressing) {
         if (current_index < meshes.size() - 1) {
             current_index += 1;
         } else {
             if ((meshes[current_index]->indices).size() > 200) {
                 meshes.emplace_back(simplify_mesh(meshes[current_index]->positions, meshes[current_index]->indices, simplification_ratio));
                 current_index += 1;
                 vertexCountIndices.push_back(current_index);
             }
         }
         is_right_pressing = false;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (init_mouse) {
        last_x = static_cast<float>(xpos);
        last_y = static_cast<float>(ypos);
        init_mouse = false;
    }

    float xoffset = static_cast<float>(xpos) - last_x;
    float yoffset = last_y - static_cast<float>(ypos);

    last_x = static_cast<float>(xpos);
    last_y = static_cast<float>(ypos);

    // base_angle_y += static_cast<float>(xoffset);
    // angle_x += static_cast<float>(yoffset);
    if(camera_movement_enabled)camera.process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if(camera_movement_enabled)camera.process_mouse_scroll(static_cast<float>(yoffset));
}
