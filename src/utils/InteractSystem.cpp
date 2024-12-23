#include "utils/InteractSystem.h"

void DrawEditor() {
    // 增大字体缩放因子
    ImGui::GetIO().FontGlobalScale = 2.0f; // 这里将字体放大 1.5 倍，你可以根据需要调整这个值
    DrawMainMenu();
    DrawResourceExplorer();
    DrawCreateNewProjectWindow();
    DrawWindowBar();    // 窗口栏
    DrawMainContent(getMeshes());  // 主内容区域
}

void DrawMainMenu() {
    if (ImGui::BeginMainMenuBar())
    {


        // File menu
        if (ImGui::BeginMenu("File"))
        {
            if (DrawHoverBorder("New", "Ctrl+N")) {
            };
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
            if (ImGui::MenuItem("Resource Explorer", NULL, &showResourceExplorer))
            {
                // 点击菜单项时，切换资源管理器窗口的显示状态
                std::cout << "showResourceExplorer" << std::endl;
                /*showResourceExplorer = !showResourceExplorer;*/
            }
            if (ImGui::MenuItem("Show Window Bar", NULL, &showWindowBar)) {
                //showWindowBar = !showWindowBar;//
            }
            ImGui::MenuItem("resource");
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
            if (ImGui::MenuItem("New Project")) {
                showCreateNewProjectWindow = true;
            };
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
                std::string name("New Window " + std::to_string(newWindowCounter));
                std::string path( "./Windows/" + std::string("New Window") + std::to_string(newWindowCounter++)+std::string(".txt"));
                CreateFileInDirectory(path);
                windows.push_back({name,path, true });
                // 设置下一个窗口的大小
                ImGui::SetNextWindowSize(ImVec2(500, 360)); // 这里设置窗口的宽度和高度
                // 显示窗口
                if (ImGui::Begin(name.c_str(), &windows.back().isOpen)) { 
                    // 在这里添加窗口的内容
                }
                ImGui::End(); // 结束窗口
            }
            ImGui::MenuItem("Close Window");
            ImGui::EndMenu();
        }
        // Help menu
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("View Help"))
            {
                windows.push_back({ "Help " ,"", true });
           }
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
        std::cout << showResourceExplorer << std::endl;
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
void DrawCreateNewProjectWindow() {
    if (showCreateNewProjectWindow) {
        static char projectName[128] = "";  // 存储项目名称

        // 创建新项目窗口
        ImGui::Begin("New Project");

        // 输入项目名称
        ImGui::BeginGroup(); // 开始分组
        ImGui::Text("Project Name:"); // 显示标签
        ImGui::SameLine(); // 保持在同一行

        // 设置初始边框宽度
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 123, 142, 255));
        // 绘制输入框
        if (ImGui::InputText("##project_name", projectName, IM_ARRAYSIZE(projectName)))
        {
            
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(); // 弹出以恢复边框宽度

        ImGui::EndGroup(); // 结束分组
        ImGui::Separator();
        // 计算按钮的垂直位置和宽度
        float buttonHeight = ImGui::GetFrameHeight(); // 获取按钮的标准高度
        float paddingFromBottom = 30; // 底部间距
        float windowWidth = ImGui::GetWindowWidth(); // 获取窗口宽度

        // 将光标移动到底部，留出一定的间距
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - paddingFromBottom - buttonHeight);

        ImGui::Separator();
        // 创建文件夹按钮
        if (ImGui::Button("Create",ImVec2(200, 40))) {
            //showCreateNewProjectWindow = false;
            if (strlen(projectName) > 0) {
                std::filesystem::path projectPath(projectName);
                if (!std::filesystem::exists(projectPath)) {
                    // 创建新文件夹
                    std::filesystem::create_directory(projectPath);
                    ImGui::OpenPopup("Create Project Success");
                }
                else {
                    ImGui::OpenPopup("Folder Exists");
                }
            }
            else {
                ImGui::OpenPopup("Input Required");
            }
        }

        // 显示弹窗
        // 设置弹窗大小为 300x200
        // 设置弹窗大小
        ImGui::SetNextWindowSize(ImVec2(300, 160));

        // 显示弹窗
        if (ImGui::BeginPopupModal("Create Project Success", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            // 显示文本
            ImGui::Text("yeah!success!!");

            // 分隔线
            ImGui::Separator();

            // 创建一个空白区域，将按钮推到底部
            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPos().y + 3)); // 向下移动光标
            ImGui::Dummy(ImVec2(0, 40)); // 创建一个高度为30的空白区域

            // 设置按钮位置
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130); // 将光标移动到窗口右侧，留出按钮宽度和一些边距
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                showCreateNewProjectWindow = false;
            }

            ImGui::EndPopup();
        }
        ImGui::SetNextWindowSize(ImVec2(300, 160));
        if (ImGui::BeginPopupModal("Folder Exists", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("already existed!!");
            ImGui::Separator();
            // 创建一个空白区域，将按钮推到底部
            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPos().y + 5)); // 向下移动光标
            ImGui::Dummy(ImVec2(0, 40)); // 创建一个高度为30的空白区域

            // 设置按钮位置
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130); // 将光标移动到窗口右侧，留出按钮宽度和一些边距
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::SetNextWindowSize(ImVec2(300, 160));
        if (ImGui::BeginPopupModal("Input Required", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("we need a valid name!!");
            ImGui::Separator();
            // 创建一个空白区域，将按钮推到底部
            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPos().y + 5)); // 向下移动光标
            ImGui::Dummy(ImVec2(0, 40)); // 创建一个高度为30的空白区域

            // 设置按钮位置
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130); // 将光标移动到窗口右侧，留出按钮宽度和一些边距
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
       
        ImGui::SameLine(); // 并排排列按钮
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 100, 0, 255)); // 设置按钮背景为红色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(200, 0, 0, 255)); // 设置按钮悬停时的背景为暗红色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(180, 0, 0, 255)); // 设置按钮按下时的背景为更暗的红色
        if (ImGui::Button("Cancel", ImVec2(200, 40))) {
            showCreateNewProjectWindow = false;
        }
        ImGui::PopStyleColor(3); // 弹出三次以恢复按钮颜色
        

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
                windows.push_back({ entry.path().filename().string().c_str(),entry.path().string().c_str(), true });
            }
        }
    }
}


// 绘制窗口栏
void DrawWindowBar() {
    if (showWindowBar) {
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
                    bool isActive = it != activeWindowIndices.end();

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
}

void DrawMainContent(std::vector<std::unique_ptr<Model>>& meshes) {
    for (int i = 0;i < activeWindowIndices.size();i++) {
        activeWindowIndex = activeWindowIndices[i];
        if (activeWindowIndex >= 0 && activeWindowIndex < windows.size()) {
            ImGui::Begin(windows[activeWindowIndex].name.c_str(), &windows[activeWindowIndex].isOpen, ImGuiWindowFlags_HorizontalScrollbar);


            std::string windowName(windows[activeWindowIndex].name.c_str());
            std::string windowPath(windows[activeWindowIndex].filepath.c_str());
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
                DisplayFileContents(windowPath);
            }
            // 获取当前窗口的大小和位置
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();

            // 设置右上角关闭按钮的大小
            ImVec2 closeButtonSize(100, 40);

            // 设置关闭按钮的相对位置（右上角）
            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImVec2(10, windowSize.y + 10));

            ImGui::End();
        }
    }
}
void DisplayFileContents(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        ImGui::Text("Failed to open file: %s", filepath.c_str());
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContents = buffer.str();

    // 动态分配缓冲区
    static char* cbuffer = new char[1024 * 1024];
    strcpy(cbuffer, fileContents.c_str()); // 将 std::string 的内容复制到缓冲区中
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    // 使用 InputTextMultiline 显示内容
    ImGui::InputTextMultiline("##filecontents", cbuffer, 1024*1024,availableSize);
    std::string fileContentsChanged(cbuffer);
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_S))
    {
        SaveToFile(filepath, fileContentsChanged); // 保存文件
    }

    file.close();
}