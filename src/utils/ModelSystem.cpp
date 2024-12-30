#include "ModelSystem.h"

// 变量定义
std::vector<int> vertexCountIndices;
std::vector<std::unique_ptr<Model>> meshes;
vecf3 pos = {0.0f,0.0f,0.0f};
int current_index = 0;
float simplification_ratio = 0.99f;
bool shows_border = false;
bool isInsideVertexCountsWindow = false;
bool isButtonClicked = false;
bool isRotating = false;

// Getter 函数定义
std::vector<int>* getVertexCountIndices() {
    return &vertexCountIndices;
}

std::vector<std::unique_ptr<Model>>& getMeshes() {
    return meshes;
}

int* getCurrentIndex() {
    return &current_index;
}

float* getSimplificationRatio() {
    return &simplification_ratio;
}

bool* getShowsBorder() {
    return &shows_border;
}

bool* getIsInsideVertexCountsWindow() {
    return &isInsideVertexCountsWindow;
}

bool* getIsButtonClicked() {
    return &isButtonClicked;
}


//加载网格体
void load_meshes() {
	//meshes.emplace_back(Model::load(RESOURCES_DIR"/squirrel.obj"));
    std::cout << "songshu1" << std::endl;
    pos = { 0.0f,0.0f,0.0f };
    meshes.emplace_back(Model::load(RESOURCES_DIR"/tuzi.obj"));
    std::cout << "songshu2" << std::endl;

	vertexCountIndices.push_back(0);
}

void DrawModel(Shader shader,Shader border_shader) {
    // render the model
    vecf3 model_pos(pos);
    static float angle_y;
    if(isRotating)angle_y= base_angle_y + static_cast<float>(glfwGetTime());
    // rot matrix
    matf4 model_transform;
    model_transform << cos(angle_y), 0.0f, sin(angle_y), model_pos[0],
        0.0f, 1.0f, 0.0f, model_pos[1],
        -sin(angle_y), 0.0f, cos(angle_y), model_pos[2],
        0.0f, 0.0f, 0.0f, 1.0f;
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
}