#pragma once

#include <glew.h>
#include <glfw3.h>
#include "Camera.h"
#include "LightManager.h"

class InputManager {
public:
    InputManager(Camera& camera, LightManager& lightManager);
    void processInput(GLFWwindow* window, float deltaTime);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    Camera& camera;
    LightManager& lightManager;
    bool cursorVisible = false;
    bool firstMouse = true;
    float lastX = 400, lastY = 300;
    void toggleCursorVisibility(GLFWwindow* window);
    void handleKeyInput(GLFWwindow* window, float deltaTime);
    void toggleWireframeMode();
};
