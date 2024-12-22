#version 330 core
out vec4 GridFragColor;


uniform vec3 gridColor;

void main() {
    GridFragColor = vec4( gridColor , 1.0); // 网格颜色可调
}
