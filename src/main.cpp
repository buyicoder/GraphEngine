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

using Utils::Camera;
using Utils::Shader;
using Utils::Model;

bool DetectMouseOutsideClick() {
    ImGuiIO& io = ImGui::GetIO();

    // 如果鼠标未被 ImGui 捕获，且发生点击
    if (!io.WantCaptureMouse) {
        std::cout << "Mouse clicked outside all ImGui windows!" << std::endl;
        return true;
    }
    else {
        std::cout << ImGui::IsMouseClicked(0) << " " << !io.WantCaptureMouse << std::endl;
        std::cout << "Mouse clicked inside an ImGui window." << std::endl;
        return false;
    }
}


namespace fs = std::filesystem;

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
bool DrawHoverBorder(const char* label, const char* shortcut = nullptr);
void SetupImGuiStyle();
void DrawResourceExplorer();
void DisplayDirectoryContents(const std::string& path);
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

//windows
// 全局变量，控制资源管理器窗口是否显示
bool showResourceExplorer = false;
bool showWindowBar = false;
// 当前浏览的文件夹路径（默认值为当前工作目录）
std::string currentPath = fs::current_path().string();


//窗口栏
// 定义一个窗口结构
struct Window {
    std::string name;    // 窗口名称
    bool isOpen;         // 窗口是否打开
};

// 全局变量，保存窗口列表
std::vector<Window> windows = {
    {"Home", true},      // 默认一个 Home 窗口
    {"Settings", true},   // 默认一个 Settings 窗口
    {"Attribute", true }
};

// 当前激活的窗口索引
int activeWindowIndex = 0;


    

// 绘制窗口栏
void DrawWindowBar();

// 绘制主内容区域
void DrawMainContent(std::vector<std::unique_ptr<Model>>& meshes);

// 主菜单栏
void DrawMenuBar();


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

        
        DrawResourceExplorer();
        DrawWindowBar();    // 窗口栏
        DrawMainContent(meshes);  // 主内容区域
        // Set custom style colors for the menu bar
        //SetupImGuiStyle();
        // Start the menu bar
        if (ImGui::BeginMainMenuBar())
        {


            // File menu
            if (ImGui::BeginMenu("File"))
            {
                if (DrawHoverBorder("New", "Ctrl+N"));
                if (DrawHoverBorder("Open", "Ctrl+O"));
                if (DrawHoverBorder("Save", "Ctrl+S"));
                ImGui::Separator();
                if (DrawHoverBorder("Exit", "Alt+F4")) {
                    glfwSetWindowShouldClose(window, true); // 设置窗口应关闭
                };
                ImGui::EndMenu();
            }

            // Edit menu
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo", "Ctrl+Z");
                ImGui::MenuItem("Redo", "Ctrl+Y");
                ImGui::Separator();
                ImGui::MenuItem("Cut", "Ctrl+X");
                ImGui::MenuItem("Copy", "Ctrl+C");
                ImGui::MenuItem("Paste", "Ctrl+V");
                ImGui::EndMenu();
            }

            // View menu
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Resource Explorer", NULL, showResourceExplorer))
                {
                    // 点击菜单项时，切换资源管理器窗口的显示状态
                    showResourceExplorer = !showResourceExplorer;
                }
                if (ImGui::MenuItem("Show Window Bar", NULL, showWindowBar)) {
                    showWindowBar = !showWindowBar;
                }
                ImGui::MenuItem("Zoom In", "Ctrl++");
                ImGui::MenuItem("Zoom Out", "Ctrl+-");
                ImGui::MenuItem("Reset Zoom", "Ctrl+0");
                ImGui::EndMenu();
            }

            // Git menu
            if (ImGui::BeginMenu("Git"))
            {
                ImGui::MenuItem("Commit");
                ImGui::MenuItem("Push");
                ImGui::MenuItem("Pull");
                ImGui::EndMenu();
            }

            // Project menu
            if (ImGui::BeginMenu("Project"))
            {
                ImGui::MenuItem("Add Files");
                ImGui::MenuItem("Remove Files");
                ImGui::MenuItem("Properties");
                ImGui::EndMenu();
            }

            // Build menu
            if (ImGui::BeginMenu("Build"))
            {
                ImGui::MenuItem("Build Solution", "F7");
                ImGui::MenuItem("Rebuild Solution");
                ImGui::MenuItem("Clean Solution");
                ImGui::EndMenu();
            }

            // Debug menu
            if (ImGui::BeginMenu("Debug"))
            {
                ImGui::MenuItem("Start Debugging", "F5");
                ImGui::MenuItem("Step Into", "F11");
                ImGui::MenuItem("Step Over", "F10");
                ImGui::MenuItem("Stop Debugging", "Shift+F5");
                ImGui::EndMenu();
            }

            // Test menu
            if (ImGui::BeginMenu("Test"))
            {
                ImGui::MenuItem("Run Tests");
                ImGui::MenuItem("Debug Tests");
                ImGui::EndMenu();
            }

            // Analyze menu
            if (ImGui::BeginMenu("Analyze"))
            {
                ImGui::MenuItem("Code Metrics");
                ImGui::MenuItem("Run Profiler");
                ImGui::EndMenu();
            }

            // Tools menu
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Options");
                ImGui::MenuItem("Extensions");
                ImGui::EndMenu();
            }

            // Extensions menu
            if (ImGui::BeginMenu("Extensions"))
            {
                ImGui::MenuItem("Manage Extensions");
                ImGui::MenuItem("Download Extensions");
                ImGui::EndMenu();
            }

            // Window menu
            if (ImGui::BeginMenu("Window"))
            {

                if (ImGui::MenuItem("New Window")) {
                    // 添加一个新窗口
                    static int newWindowCounter = 1;
                    windows.push_back({ "New Window " + std::to_string(newWindowCounter++), true });
                }
                ImGui::MenuItem("Close Window");
                ImGui::EndMenu();
            }
            // Help menu
            if (ImGui::BeginMenu("Help"))
            {
                ImGui::MenuItem("View Help");
                ImGui::MenuItem("About");
                ImGui::EndMenu();
            }

            // End the menu bar
            ImGui::EndMainMenuBar();
        }
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
        std::cout << "press esc" << std::endl;
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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        //std::cout << "Left mouse button clicked!" << std::endl;
        if (DetectMouseOutsideClick()) {
            camera_movement_enabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            //std::cout << "has set" << std::endl;
            //std::cout << camera_movement_enabled << std::endl;
        }
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    std::cout << "into mouse_callback" << std::endl;
    std::cout << camera_movement_enabled << std::endl;
    ImGuiIO& io = ImGui::GetIO();
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
void SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // 去除窗口的边界 Padding
    style.WindowPadding = ImVec2(0.0f, 0.0f); // 窗口内的上下左右填充设置为 0

    // 去除菜单项内部的左右 Padding
    style.FramePadding = ImVec2(0.0f, 4.0f); // 高度可根据需要调整，保持文本居中

    // 调整 ItemSpacing（菜单项之间的间距）
    style.ItemSpacing = ImVec2(0.0f, 0.0f); // 菜单项之间无间距

    // 调整菜单栏的内边距
    style.WindowBorderSize = 0.0f; // 去除窗口边框

    style.IndentSpacing = 0.0f;    // 去除菜单层级缩进


    // 设置菜单背景为黑色
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // 黑色背景

    // 设置菜单悬停时的颜色（例如浅灰色）
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    // 设置菜单选中时的颜色（例如深灰色）
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    // 设置菜单项的文本颜色
    style.Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); // 浅灰色文本

    // 设置菜单边框颜色
    style.Colors[ImGuiCol_Border] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
}


  

bool DrawHoverBorder(const char* label, const char* shortcut)
{
    // Render the menu item and check if it is hovered
    bool hovered = ImGui::MenuItem(label, shortcut);

    // If hovered, draw a border around the item
    if (ImGui::IsItemHovered())
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 min = ImGui::GetItemRectMin(); // Top-left of the item
        ImVec2 max = ImGui::GetItemRectMax(); // Bottom-right of the item
        ImU32 borderColor = ImColor(150, 150, 150, 255); // Light gray border color
        float thickness = 5.0f; // Border thickness
        drawList->AddRect(min, max, borderColor, 0.0f, ImDrawFlags_RoundCornersNone, thickness);
    }
    return hovered;
}

void DrawResourceExplorer()
{
    if (showResourceExplorer)
    {
        ImGui::Begin("Resource Explorer", &showResourceExplorer, ImGuiWindowFlags_HorizontalScrollbar);

        // 显示当前路径
        ImGui::Text("Current Path: %s", currentPath.c_str());
        ImGui::Separator();

        // 返回上一级按钮
        if (ImGui::Button(".. (Up one level)"))
        {
            // 返回上一级目录
            currentPath = fs::path(currentPath).parent_path().string();
        }

        ImGui::Separator();

        // 显示当前目录的内容
        DisplayDirectoryContents(currentPath);

        ImGui::End();
    }
}
void DisplayDirectoryContents(const std::string& path)
{
    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.is_directory()) // 如果是文件夹
        {
            if (ImGui::TreeNode(entry.path().filename().string().c_str()))
            {
                // 递归显示子文件夹内容
                DisplayDirectoryContents(entry.path().string());
                ImGui::TreePop();
            }
        }
        else if (entry.is_regular_file()) // 如果是文件
        {
            // 显示文件
            if (ImGui::Selectable(entry.path().filename().string().c_str()))
            {
                // 如果点击文件，可以在这里处理点击逻辑，比如打印文件路径
                printf("File clicked: %s\n", entry.path().string().c_str());
            }
        }
    }
}


// 绘制窗口栏
void DrawWindowBar() {
    // 窗口栏
    // 定义窗口栏的位置和大小
    float menuBarHeight = ImGui::GetFrameHeight(); // 动态获取菜单栏高度
    ImVec2 windowPos = ImVec2(0, menuBarHeight);   // 将窗口栏放在菜单栏下方
    ImVec2 windowSize = ImVec2(ImGui::GetIO().DisplaySize.x, 50); // 宽度覆盖屏幕，固定高度为 30

    // 设置窗口栏的位置和大小
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);

    // 设置窗口栏窗口的标志
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |  // 无标题栏
        ImGuiWindowFlags_NoResize |    // 禁止调整大小
        ImGuiWindowFlags_NoMove |      // 禁止移动
        ImGuiWindowFlags_NoCollapse |  // 禁止折叠
        ImGuiWindowFlags_NoSavedSettings; // 不保存设置

    // 开始绘制窗口栏
    if (ImGui::Begin("Window Bar", nullptr, windowFlags)) {
        if (ImGui::BeginTabBar("WindowBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs)) {
            for (int i = 0; i < windows.size(); ++i) {
                auto& window = windows[i];

                // 如果窗口被关闭，跳过绘制
                if (!window.isOpen) continue;

                // 每个窗口作为一个 Tab
                if (ImGui::BeginTabItem(window.name.c_str(), &window.isOpen)) {
                    activeWindowIndex = i; // 设置当前激活的窗口索引
                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End(); // 结束窗口栏
    // 移除被关闭的窗口
    windows.erase(
        std::remove_if(windows.begin(), windows.end(), [](const Window& window) {
            return !window.isOpen;
            }),
        windows.end()
    );
}

void DrawMainContent(std::vector<std::unique_ptr<Model>>& meshes) {
    if (activeWindowIndex >= 0 && activeWindowIndex < windows.size()) {
        ImGui::Begin(windows[activeWindowIndex].name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);
        std::string windowName(windows[activeWindowIndex].name.c_str());
        if (windowName == "Home") {
            ImGui::Text("Welcome to the Home window!");
        }
        else if (windowName == "Settings") {
            ImGui::Text("This is the Settings window.");
        }
        else if (windowName == "Attribute") {
            


            ImGui::Text("vertices: %d", meshes[current_index]->positions.size());
            ImGui::Text("faces: %d", meshes[current_index]->indices.size());
            ImGui::Text("borders: %s", shows_border ? "On" : "Off");

            static int counter = 0;
            static float f = 0.0f;

            if (ImGui::Button("Button")) {
                ImGui::SameLine();
            }
            ImGui::Text("counter = %d", counter);

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);


            for (size_t i = 0; i < vertexCountIndices.size(); ++i) {
                std::string buttonLabel = std::to_string(meshes[vertexCountIndices[i]]->positions.size());
                if (ImGui::TreeNodeEx(buttonLabel.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth)) {
                    // 当按钮被点击时，切换到相应顶点数的模型
                    current_index = i;
                    isButtonClicked = true;

                    // 在树节点展开时显示额外的信息
                    ImGui::Text("Additional Information:");
                    ImGui::Text("Vertices: %d", meshes[vertexCountIndices[i]]->positions.size());
                    ImGui::Text("Faces: %d", meshes[vertexCountIndices[i]]->indices.size());

                    ImGui::TreePop(); // 关闭树节点
                }
                // 检查鼠标是否在当前按钮上
                if (ImGui::IsItemClicked(0)) {
                    isInsideVertexCountsWindow = true;
                    std::cout << buttonLabel.c_str() << " was clicked" << std::endl;
                    current_index = i;
                    isButtonClicked = true;
                }
                //std::cout << "current_index has been " << vertexCountIndices[i] << std::endl;
               
            }


   
        }
        else {
            ImGui::Text("This is a dynamically created window: %s", windowName.c_str());
        }
        ImGui::End();
    }
}
