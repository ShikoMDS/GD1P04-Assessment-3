#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <unordered_map>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "LightManager.h"
#include "Model.h"
#include "Skybox.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // Set initial position a bit further back to allow zooming in and out
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Lighting
LightManager lightManager;

// Debounce state
std::unordered_map<int, bool> keyState = {
    {GLFW_KEY_1, false},
    {GLFW_KEY_2, false},
    {GLFW_KEY_3, false},
    {GLFW_KEY_4, false},
    {GLFW_KEY_5, false}
};
std::unordered_map<int, bool> keyToggleState;

// Wireframe mode
bool wireframe = false;

// Cursor visibility
bool cursorVisible = false;

// Scale factor for the model
const float modelScaleFactor = 0.01f;  // Adjust this value to scale down the model

void processInput(GLFWwindow* window);
void toggleWireframeMode();
void toggleCursorVisibility(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
    // Initialize and configure GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Enable MSAA

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Demo", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // Enable back-face culling
    glEnable(GL_MULTISAMPLE); // Enable MSAA

    // Build and compile shaders
    Shader lightingShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag");
    Shader reflectionShader("resources/shaders/ReflectionVertexShader.vert", "resources/shaders/ReflectionFragmentShader.frag");
    Shader skyboxShader("resources/shaders/SkyboxVertexShader.vert", "resources/shaders/SkyboxFragmentShader.frag");

    // Load models
    Model gardenPlant("resources/models/AncientEmpire/SM_Env_Garden_Plants_01.obj", "resources/textures/PolygonAncientWorlds_Texture_01_A.png");
    Model tree("resources/models/AncientEmpire/SM_Env_Tree_Palm_01.obj", "resources/textures/PolygonAncientWorlds_Texture_01_A.png");
    Model statue("resources/models/AncientEmpire/SM_Prop_Statue_01.obj", "resources/textures/PolygonAncientWorlds_Texture_01_A.png");

    // Load skybox
    std::vector<std::string> faces
    {
        "resources/skybox/Corona/Right.png",   // Right
        "resources/skybox/Corona/Left.png",    // Left
        "resources/skybox/Corona/Top.png",     // Top
        "resources/skybox/Corona/Bottom.png",  // Bottom
        "resources/skybox/Corona/Back.png",    // Back
        "resources/skybox/Corona/Front.png"    // Front
    };
    Skybox skybox(faces);

    // Initialize lighting
    lightManager.initialize();

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set shader and pass uniforms
        lightingShader.use();
        lightingShader.setMat4("view", camera.getViewMatrix());
        lightingShader.setMat4("projection", camera.getProjectionMatrix(SCR_WIDTH, SCR_HEIGHT));
        lightingShader.setVec3("viewPos", camera.Position);

        // Set lighting uniforms
        lightManager.updateLighting(lightingShader);

        // Set additional uniforms for lights
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);

        // Render garden plants
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.0f, 0.0f, -2.0f));  // Adjust position
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));  // Apply scaling
        lightingShader.setMat4("model", modelMatrix);
        gardenPlant.Draw(lightingShader);

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, -2.0f));  // Adjust position
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));  // Apply scaling
        lightingShader.setMat4("model", modelMatrix);
        gardenPlant.Draw(lightingShader);

        // Render trees
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.0f, 0.0f, 2.0f));  // Adjust position
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));  // Apply scaling
        lightingShader.setMat4("model", modelMatrix);
        tree.Draw(lightingShader);

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, 2.0f));  // Adjust position
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));  // Apply scaling
        lightingShader.setMat4("model", modelMatrix);
        tree.Draw(lightingShader);

        // Render statue in the middle
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));  // Center position
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));  // Apply scaling
        lightingShader.setMat4("model", modelMatrix);
        statue.Draw(lightingShader);

        // Render the reflective model
        reflectionShader.use();
        reflectionShader.setMat4("view", camera.getViewMatrix());
        reflectionShader.setMat4("projection", camera.getProjectionMatrix(SCR_WIDTH, SCR_HEIGHT));
        reflectionShader.setVec3("cameraPos", camera.Position);
        reflectionShader.setMat4("model", modelMatrix); // Assuming same model matrix for simplicity
        reflectionShader.setBool("useTexture", true);
        reflectionShader.setInt("skybox", 0); // Bind the skybox texture to unit 0
        statue.Draw(reflectionShader);  // Changed the reflective model to the statue

        // Render skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
        skyboxShader.setMat4("projection", camera.getProjectionMatrix(SCR_WIDTH, SCR_HEIGHT));
        skybox.Draw(skyboxShader);
        glDepthFunc(GL_LESS);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
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

void toggleWireframeMode()
{
    wireframe = !wireframe;
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void toggleCursorVisibility(GLFWwindow* window)
{
    cursorVisible = !cursorVisible;
    if (cursorVisible) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true; // Reset firstMouse when showing cursor
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (cursorVisible) return; // Skip camera movement when cursor is visible

    if (firstMouse)
    {
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}