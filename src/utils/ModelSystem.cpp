#include "ModelSystem.h"

// 变量定义
std::vector<int> vertexCountIndices;
std::vector<std::unique_ptr<Model>> meshes;
int current_index = 0;
float simplification_ratio = 0.99f;
bool shows_border = false;
bool isInsideVertexCountsWindow = false;
bool isButtonClicked = false;

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
	meshes.emplace_back(Model::load(RESOURCES_DIR"/squirrel.obj"));
	vertexCountIndices.push_back(0);
}