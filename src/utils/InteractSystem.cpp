#include "utils/InteractSystem.h"

void DrawEditor() {
    // ����������������
    ImGui::GetIO().FontGlobalScale = 2.0f; // ���ｫ����Ŵ� 1.5 ��������Ը�����Ҫ�������ֵ
    DrawMainMenu();
    DrawResourceExplorer();
    DrawCreateNewProjectWindow();
    DrawWindowBar();    // ������
    DrawMainContent(getMeshes());  // ����������
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
                glfwSetWindowShouldClose(window, true); // ���ô���Ӧ�ر�
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
                // ����˵���ʱ���л���Դ���������ڵ���ʾ״̬
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
                // ���һ���´���
                
                static int newWindowCounter = 1;
                std::string name("New Window " + std::to_string(newWindowCounter));
                std::string path( "./Windows/" + std::string("New Window") + std::to_string(newWindowCounter++)+std::string(".txt"));
                CreateFileInDirectory(path);
                windows.push_back({name,path, true });
                // ������һ�����ڵĴ�С
                ImGui::SetNextWindowSize(ImVec2(500, 360)); // �������ô��ڵĿ�Ⱥ͸߶�
                // ��ʾ����
                if (ImGui::Begin(name.c_str(), &windows.back().isOpen)) { 
                    // ��������Ӵ��ڵ�����
                }
                ImGui::End(); // ��������
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

    // ȥ�����ڵı߽� Padding
    style.WindowPadding = ImVec2(0.0f, 0.0f); // �����ڵ����������������Ϊ 0

    // ȥ���˵����ڲ������� Padding
    style.FramePadding = ImVec2(0.0f, 4.0f); // �߶ȿɸ�����Ҫ�����������ı�����

    // ���� ItemSpacing���˵���֮��ļ�ࣩ
    style.ItemSpacing = ImVec2(0.0f, 0.0f); // �˵���֮���޼��

    // �����˵������ڱ߾�
    style.WindowBorderSize = 0.0f; // ȥ�����ڱ߿�

    style.IndentSpacing = 0.0f;    // ȥ���˵��㼶����


    // ���ò˵�����Ϊ��ɫ
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // ��ɫ����

    // ���ò˵���ͣʱ����ɫ������ǳ��ɫ��
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    // ���ò˵�ѡ��ʱ����ɫ���������ɫ��
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    // ���ò˵�����ı���ɫ
    style.Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); // ǳ��ɫ�ı�

    // ���ò˵��߿���ɫ
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

        // ��ʾ��ǰ·��
        ImGui::Text("Current Path: %s", currentPath.c_str());
        ImGui::Separator();

        // ������һ����ť
        if (ImGui::Button(".. (Up one level)"))
        {
            // ������һ��Ŀ¼
            currentPath = std::filesystem::path(currentPath).parent_path().string();
        }

        ImGui::Separator();

        // ��ʾ��ǰĿ¼������
        DisplayDirectoryContents(currentPath);

        ImGui::End();
    }
}
void DrawCreateNewProjectWindow() {
    if (showCreateNewProjectWindow) {
        static char projectName[128] = "";  // �洢��Ŀ����

        // ��������Ŀ����
        ImGui::Begin("New Project");

        // ������Ŀ����
        ImGui::BeginGroup(); // ��ʼ����
        ImGui::Text("Project Name:"); // ��ʾ��ǩ
        ImGui::SameLine(); // ������ͬһ��

        // ���ó�ʼ�߿���
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 123, 142, 255));
        // ���������
        if (ImGui::InputText("##project_name", projectName, IM_ARRAYSIZE(projectName)))
        {
            
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(); // �����Իָ��߿���

        ImGui::EndGroup(); // ��������
        ImGui::Separator();
        // ���㰴ť�Ĵ�ֱλ�úͿ��
        float buttonHeight = ImGui::GetFrameHeight(); // ��ȡ��ť�ı�׼�߶�
        float paddingFromBottom = 30; // �ײ����
        float windowWidth = ImGui::GetWindowWidth(); // ��ȡ���ڿ��

        // ������ƶ����ײ�������һ���ļ��
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - paddingFromBottom - buttonHeight);

        ImGui::Separator();
        // �����ļ��а�ť
        if (ImGui::Button("Create",ImVec2(200, 40))) {
            //showCreateNewProjectWindow = false;
            if (strlen(projectName) > 0) {
                std::filesystem::path projectPath(projectName);
                if (!std::filesystem::exists(projectPath)) {
                    // �������ļ���
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

        // ��ʾ����
        // ���õ�����СΪ 300x200
        // ���õ�����С
        ImGui::SetNextWindowSize(ImVec2(300, 160));

        // ��ʾ����
        if (ImGui::BeginPopupModal("Create Project Success", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            // ��ʾ�ı�
            ImGui::Text("yeah!success!!");

            // �ָ���
            ImGui::Separator();

            // ����һ���հ����򣬽���ť�Ƶ��ײ�
            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPos().y + 3)); // �����ƶ����
            ImGui::Dummy(ImVec2(0, 40)); // ����һ���߶�Ϊ30�Ŀհ�����

            // ���ð�ťλ��
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130); // ������ƶ��������Ҳ࣬������ť��Ⱥ�һЩ�߾�
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
            // ����һ���հ����򣬽���ť�Ƶ��ײ�
            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPos().y + 5)); // �����ƶ����
            ImGui::Dummy(ImVec2(0, 40)); // ����һ���߶�Ϊ30�Ŀհ�����

            // ���ð�ťλ��
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130); // ������ƶ��������Ҳ࣬������ť��Ⱥ�һЩ�߾�
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::SetNextWindowSize(ImVec2(300, 160));
        if (ImGui::BeginPopupModal("Input Required", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("we need a valid name!!");
            ImGui::Separator();
            // ����һ���հ����򣬽���ť�Ƶ��ײ�
            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPos().y + 5)); // �����ƶ����
            ImGui::Dummy(ImVec2(0, 40)); // ����һ���߶�Ϊ30�Ŀհ�����

            // ���ð�ťλ��
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130); // ������ƶ��������Ҳ࣬������ť��Ⱥ�һЩ�߾�
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
       
        ImGui::SameLine(); // �������а�ť
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 100, 0, 255)); // ���ð�ť����Ϊ��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(200, 0, 0, 255)); // ���ð�ť��ͣʱ�ı���Ϊ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(180, 0, 0, 255)); // ���ð�ť����ʱ�ı���Ϊ�����ĺ�ɫ
        if (ImGui::Button("Cancel", ImVec2(200, 40))) {
            showCreateNewProjectWindow = false;
        }
        ImGui::PopStyleColor(3); // ���������Իָ���ť��ɫ
        

        ImGui::End();
        
    }
}
void DisplayDirectoryContents(const std::string& path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory()) // ������ļ���
        {
            // ��ʾĿ¼
            if (ImGui::Button(entry.path().filename().string().c_str()))
            {
                currentPath = entry.path().string().c_str();  // ���Ŀ¼����
            }
        }
        else if (entry.is_regular_file()) // ������ļ�
        {
            // ��ʾ�ļ�
            if (ImGui::Selectable(entry.path().filename().string().c_str()))
            {
                windows.push_back({ entry.path().filename().string().c_str(),entry.path().string().c_str(), true });
            }
        }
    }
}


// ���ƴ�����
void DrawWindowBar() {
    if (showWindowBar) {
        // ������
        // ���崰������λ�úʹ�С
        float menuBarHeight = ImGui::GetFrameHeight(); // ��̬��ȡ�˵����߶�
        ImVec2 windowPos = ImVec2(0, menuBarHeight);   // �����������ڲ˵����·�
        ImVec2 windowSize = ImVec2(ImGui::GetIO().DisplaySize.x, 50); // ��ȸ�����Ļ���̶��߶�Ϊ 30

        // ���ô�������λ�úʹ�С
        ImGui::SetNextWindowPos(windowPos);
        ImGui::SetNextWindowSize(windowSize);

        // ���ô��������ڵı�־
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |  // �ޱ�����
            ImGuiWindowFlags_NoResize |    // ��ֹ������С
            ImGuiWindowFlags_NoMove |      // ��ֹ�ƶ�
            ImGuiWindowFlags_NoCollapse |  // ��ֹ�۵�
            ImGuiWindowFlags_NoSavedSettings; // ����������

        // ��ʼ���ƴ�����
        if (ImGui::Begin("Window Bar", nullptr, windowFlags)) {
            if (ImGui::BeginTabBar("WindowBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs)) {
                for (int i = 0; i < windows.size(); ++i) {
                    auto& window = windows[i];

                    // ������ڱ��رգ���������
                    if (!window.isOpen) continue;

                    auto it = std::find(activeWindowIndices.begin(), activeWindowIndices.end(), i);
                    bool isActive = it != activeWindowIndices.end();

                    if (ImGui::BeginTabItem(window.name.c_str(), &window.isOpen)) {
                        // �����ȡ������ô���
                        if (!isActive) {
                            activeWindowIndices.push_back(i); // �����
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
        ImGui::End(); // ����������
        // �Ƴ����رյĴ���
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
                        // ����ť�����ʱ���л�����Ӧ��������ģ��
                        current_index = i;

                        // �����ڵ�չ��ʱ��ʾ�������Ϣ
                        ImGui::Text("Additional Information:");
                        ImGui::Text("Vertices: %d", meshes[vertexCountIndices[i]]->positions.size());
                        ImGui::Text("Faces: %d", meshes[vertexCountIndices[i]]->indices.size());

                        ImGui::TreePop(); // �ر����ڵ�
                    }
                    // �������Ƿ��ڵ�ǰ��ť��
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
            // ��ȡ��ǰ���ڵĴ�С��λ��
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();

            // �������Ͻǹرհ�ť�Ĵ�С
            ImVec2 closeButtonSize(100, 40);

            // ���ùرհ�ť�����λ�ã����Ͻǣ�
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

    // ��̬���仺����
    static char* cbuffer = new char[1024 * 1024];
    strcpy(cbuffer, fileContents.c_str()); // �� std::string �����ݸ��Ƶ���������
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    // ʹ�� InputTextMultiline ��ʾ����
    ImGui::InputTextMultiline("##filecontents", cbuffer, 1024*1024,availableSize);
    std::string fileContentsChanged(cbuffer);
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_S))
    {
        SaveToFile(filepath, fileContentsChanged); // �����ļ�
    }

    file.close();
}