#include <utils/SceneManageSystem.h>
std::vector<float> GenerateGridVertices(float gridSize, float cellSize) {
    std::vector<float> vertices;
    for (float i = -gridSize; i <= gridSize; i += cellSize) {
        // 垂直线（沿 X 轴）
        vertices.push_back(i);
        vertices.push_back(0.0f); // Y 坐标固定为 0
        vertices.push_back(-gridSize);

        vertices.push_back(i);
        vertices.push_back(0.0f);
        vertices.push_back(gridSize);

        // 水平线（沿 Z 轴）
        vertices.push_back(-gridSize);
        vertices.push_back(0.0f);
        vertices.push_back(i);

        vertices.push_back(gridSize);
        vertices.push_back(0.0f);
        vertices.push_back(i);
    }
    return vertices;
}
