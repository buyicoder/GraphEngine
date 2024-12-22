#include <utils/SceneManageSystem.h>
std::vector<float> GenerateGridVertices(float gridSize, float cellSize) {
    std::vector<float> vertices;
    for (float i = -gridSize; i <= gridSize; i += cellSize) {
        // ��ֱ�ߣ��� X �ᣩ
        vertices.push_back(i);
        vertices.push_back(0.0f); // Y ����̶�Ϊ 0
        vertices.push_back(-gridSize);

        vertices.push_back(i);
        vertices.push_back(0.0f);
        vertices.push_back(gridSize);

        // ˮƽ�ߣ��� Z �ᣩ
        vertices.push_back(-gridSize);
        vertices.push_back(0.0f);
        vertices.push_back(i);

        vertices.push_back(gridSize);
        vertices.push_back(0.0f);
        vertices.push_back(i);
    }
    return vertices;
}
