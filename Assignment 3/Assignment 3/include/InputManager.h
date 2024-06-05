#pragma once

#include <glew.h>
#include <glfw3.h>
#include <unordered_map>
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
    std::unordered_map<int, bool> keyState;
    std::unordered_map<int, bool> keyToggleState;
    bool wireframe;
    bool cursorVisible;
    float lastX;
    float lastY;
    bool firstMouse;

    void toggleWireframeMode();
    void toggleCursorVisibility(GLFWwindow* window);
};

