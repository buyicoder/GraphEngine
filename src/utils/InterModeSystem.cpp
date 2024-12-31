#include <utils/InterModeSystem.h>

int InterMode = GLOBALEDIT;

void SetInterMode() {
	if (InterMode == SCENEEXCURISION) {//��������ģʽ
		currentCamera = MAINCAMERA;
		camera_movement_enabled = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (InterMode == GLOBALEDIT) {//ȫ�ֱ༭ģʽ
		camera_movement_enabled = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (InterMode == TEXTEDIT) {//���ֱ༭ģʽ
		camera_movement_enabled = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}