#pragma once

#include <glew.h>
#include <glfw3.h>
#include "Camera.h"
#include "LightManager.h"
#include <unordered_map>

class InputManager
{
public:
    InputManager(Camera& camera, LightManager& lightManager);

    void processInput(GLFWwindow* window, float deltaTime);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void toggleWireframeMode();
    void toggleCursorVisibility(GLFWwindow* window);

private:
    Camera& camera;
    LightManager& lightManager;
    bool wireframe;
    bool cursorVisible;
    bool firstMouse;
    float lastX;
    float lastY;

    std::unordered_map<int, bool> keyState;
    std::unordered_map<int, bool> keyToggleState;
};
