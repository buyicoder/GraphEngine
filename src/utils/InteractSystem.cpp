#include "utils/InteractSystem.h"

void DrawEditor() {
    // 增大字体缩放因子
    ImGui::GetIO().FontGlobalScale = 2.0f; // 这里将字体放大 1.5 倍，你可以根据需要调整这个值
    DrawMainMenu();
    DrawResourceExplorer();
    DrawWindowBar();    // 窗口栏
    DrawMainContent(getMeshes());  // 主内容区域
}

void DrawMainMenu() {
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
            if (ImGui::MenuItem("Resource Explorer", NULL, getShowResourceExplorer()))
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
            currentPath = std::filesystem::path(currentPath).parent_path().string();
        }

        ImGui::Separator();

        // 显示当前目录的内容
        DisplayDirectoryContents(currentPath);

        ImGui::End();
    }
}
void DisplayDirectoryContents(const std::string& path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory()) // 如果是文件夹
        {
            // 显示目录
            if (ImGui::Button(entry.path().filename().string().c_str()))
            {
                currentPath = entry.path().string().c_str();  // 点击目录进入
            }
        }
        else if (entry.is_regular_file()) // 如果是文件
        {
            // 显示文件
            if (ImGui::Selectable(entry.path().filename().string().c_str()))
            {
                windows.push_back({ entry.path().filename().string().c_str(), true });
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

                auto it = std::find(activeWindowIndices.begin(), activeWindowIndices.end(), i);
                bool isActive =  it!= activeWindowIndices.end();

                if (ImGui::BeginTabItem(window.name.c_str(), &window.isOpen)) {
                    // 激活或取消激活该窗口
                    if (!isActive) {
                        activeWindowIndices.push_back(i); // 激活窗口
                    }
                    else {
                        //activeWindowIndices.erase(it);
                    }
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
    for (int i = 0;i < activeWindowIndices.size();i++) {
        activeWindowIndex = activeWindowIndices[i];
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
                    }
                    //std::cout << "current_index has been " << vertexCountIndices[i] << std::endl;

                }



            }
            else {
                ImGui::Text("This is a dynamically created window: %s", windowName.c_str());
            }
            // 获取当前窗口的大小和位置
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();

            // 设置右上角关闭按钮的大小
            ImVec2 closeButtonSize(100, 40);

            // 设置关闭按钮的相对位置（右上角）
            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImVec2(10, windowSize.y + 10));

            // 创建一个“关闭”按钮，放在窗口的左下角
            if (ImGui::Button("EXIT", ImVec2(closeButtonSize.x, closeButtonSize.y)))
            {
                windows[activeWindowIndex].isOpen = false;  // 关闭窗口
            }
            ImGui::End();
        }
    }
}
