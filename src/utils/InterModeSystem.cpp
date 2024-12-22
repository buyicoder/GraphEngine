#include <utils/InterModeSystem.h>

int InterMode = 0;

void SetInterMode() {
	if (InterMode == SCENEEXCURISION) {//场景游览模式
		camera_movement_enabled = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (InterMode == GLOBALEDIT) {//全局编辑模式
		camera_movement_enabled = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (InterMode == TEXTEDIT) {//文字编辑模式
		camera_movement_enabled = true;
	}
}
