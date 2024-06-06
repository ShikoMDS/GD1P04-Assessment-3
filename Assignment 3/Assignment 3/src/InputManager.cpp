#include "InputManager.h"
#include <iostream>

InputManager::InputManager(Camera& camera, LightManager& lightManager)
    : camera(camera), lightManager(lightManager) {}

void InputManager::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);  // Down movement
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);    // Up movement
    }

    handleKeyInput(window, deltaTime);
}

void InputManager::toggleCursorVisibility(GLFWwindow* window) {
    cursorVisible = !cursorVisible;
    if (cursorVisible) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true; // Reset firstMouse when showing cursor
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void InputManager::handleKeyInput(GLFWwindow* window, float deltaTime) {
    static bool lastState[5] = { false, false, false, false, false };
    bool currentState[5] = {
        glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS,
        glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS,
        glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS,
        glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS,
        glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS
    };

    if (currentState[0] && !lastState[0]) {
        lightManager.togglePointLights();
    }
    if (currentState[1] && !lastState[1]) {
        lightManager.toggleDirectionalLight();
    }
    if (currentState[2] && !lastState[2]) {
        lightManager.toggleSpotLight();
    }
    if (currentState[3] && !lastState[3]) {
        toggleWireframeMode();
    }
    if (currentState[4] && !lastState[4]) {
        toggleCursorVisibility(window);
    }

    std::copy(std::begin(currentState), std::end(currentState), std::begin(lastState));
}

void InputManager::toggleWireframeMode() {
    static bool wireframe = false;
    wireframe = !wireframe;
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void InputManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (cursorVisible) return; // Ignore mouse movements when cursor is visible

    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
