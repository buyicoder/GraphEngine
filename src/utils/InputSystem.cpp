#include "InputSystem.h"

// 输入系统：状态
bool is_left_pressing = false;
bool is_right_pressing = false;
bool is_b_pressing = false;
bool is_changing = false;



void set_callbacks() {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void error_callback(int code, const char* description) {
    std::cerr << "[E]" << code << " : " << description << std::endl;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //std::cout << "press esc" << std::endl;
        InterMode = GLOBALEDIT;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (camera_movement_enabled)camera.process_keyboard(Camera::Movement::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (camera_movement_enabled)camera.process_keyboard(Camera::Movement::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (camera_movement_enabled)camera.process_keyboard(Camera::Movement::LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (camera_movement_enabled)camera.process_keyboard(Camera::Movement::RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (camera_movement_enabled)camera.process_keyboard(Camera::Movement::UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (camera_movement_enabled)camera.process_keyboard(Camera::Movement::DOWN, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        is_b_pressing = true;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        is_left_pressing = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        is_right_pressing = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        is_changing = true;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && is_b_pressing) {
        is_b_pressing = false;
        shows_border = !shows_border;
    }


    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) && is_left_pressing) {
        if (current_index > 0) {
            current_index -= 1;
        }
        is_left_pressing = false;
    }
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) && is_right_pressing) {
        if (current_index < meshes.size() - 1) {
            current_index += 1;
        }
        else {
            if ((meshes[current_index]->indices).size() > 200) {
                meshes.emplace_back(simplify_mesh(meshes[current_index]->positions, meshes[current_index]->indices, simplification_ratio));
                current_index += 1;
                vertexCountIndices.push_back(current_index);
            }
        }
        is_right_pressing = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        //std::cout << "Left mouse button clicked!" << std::endl;
        if (DetectMouseOutsideClick()) {
            InterMode = SCENEEXCURISION;
            //std::cout << "has set" << std::endl;
            //std::cout << camera_movement_enabled << std::endl;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << "into mouse_callback" << std::endl;
    //std::cout << camera_movement_enabled << std::endl;
    ImGuiIO& io = ImGui::GetIO();
    if (init_mouse) {
        last_x = static_cast<float>(xpos);
        last_y = static_cast<float>(ypos);
        init_mouse = false;
    }

    float xoffset = static_cast<float>(xpos) - last_x;
    float yoffset = last_y - static_cast<float>(ypos);

    last_x = static_cast<float>(xpos);
    last_y = static_cast<float>(ypos);

    // base_angle_y += static_cast<float>(xoffset);
    // angle_x += static_cast<float>(yoffset);
    if (camera_movement_enabled)camera.process_mouse_movement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (camera_movement_enabled)camera.process_mouse_scroll(static_cast<float>(yoffset));
}
bool DetectMouseOutsideClick() {
    ImGuiIO& io = ImGui::GetIO();
    // 如果鼠标未被 ImGui 捕获，且发生点击
    if (!io.WantCaptureMouse) {
        //std::cout << "Mouse clicked outside all ImGui windows!" << std::endl;
        return true;
    }
    else {
        //std::cout << ImGui::IsMouseClicked(0) << " " << !io.WantCaptureMouse << std::endl;
        //std::cout << "Mouse clicked inside an ImGui window." << std::endl;
        return false;
    }
}