#include "InputManager.h"

InputManager::InputManager(Camera& camera, LightManager& lightManager)
    : camera(camera), lightManager(lightManager), wireframe(false), cursorVisible(false),
      lastX(800 / 2.0f), lastY(600 / 2.0f), firstMouse(true) {
    keyState = {
        {GLFW_KEY_1, false},
        {GLFW_KEY_2, false},
        {GLFW_KEY_3, false},
        {GLFW_KEY_4, false},
        {GLFW_KEY_5, false}
    };
}

void InputManager::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Debounce toggles
    for (auto& [key, pressed] : keyState) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            if (!pressed) {
                pressed = true;
                keyToggleState[key] = !keyToggleState[key];
                if (key == GLFW_KEY_1) lightManager.togglePointLights();
                if (key == GLFW_KEY_2) lightManager.toggleDirectionalLight();
                if (key == GLFW_KEY_3) lightManager.toggleSpotLight();
                if (key == GLFW_KEY_4) toggleWireframeMode();
                if (key == GLFW_KEY_5) toggleCursorVisibility(window);
            }
        }
        else {
            pressed = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void InputManager::toggleWireframeMode() {
    wireframe = !wireframe;
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void InputManager::toggleCursorVisibility(GLFWwindow* window) {
    cursorVisible = !cursorVisible;
    if (cursorVisible) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true; // Reset firstMouse when showing cursor
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void InputManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (cursorVisible) return; // Skip camera movement when cursor is visible

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
