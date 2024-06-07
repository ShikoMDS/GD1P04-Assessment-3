#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "LightManager.h"
#include "Model.h"
#include "Skybox.h"
#include "InputManager.h"

// Settings
constexpr unsigned int ScrWidth = 800;
constexpr unsigned int ScrHeight = 600;

// Camera
Camera GCamera(glm::vec3(0.0f, 0.0f, 5.0f)); // Set initial position a bit further back to allow zooming in and out

// Timing
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

// Lighting
LightManager GLightManager;

// Input Manager
InputManager inputManager(GCamera, GLightManager);

// Scale factors for the models
constexpr float ModelScaleFactor = 0.01f;  // Adjust this value to scale down the model
constexpr float PlantScaleFactor = 0.005f; // Smaller scale for garden plants
constexpr float TreeScaleFactor = 1.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    inputManager.framebufferSizeCallback(window, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    inputManager.mouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    inputManager.scrollCallback(window, xoffset, yoffset);
}

void checkGLError(const std::string& location)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
    }
}

int main()
{
    // Initialize and configure GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Enable MSAA

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* Window = glfwCreateWindow(ScrWidth, ScrHeight, "OpenGL Demo", nullptr, nullptr);
    if (!Window)
    {
        std::cerr << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetCursorPosCallback(Window, mouse_callback);
    glfwSetScrollCallback(Window, scroll_callback);

    // Capture the mouse
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << '\n';
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // Enable back-face culling
    glEnable(GL_MULTISAMPLE); // Enable MSAA

    // Build and compile shaders
    Shader LightingShader("resources/shaders/VertexShader.vert", "resources/shaders/FragmentShader.frag");
    Shader SkyboxShader("resources/shaders/SkyboxVertexShader.vert", "resources/shaders/SkyboxFragmentShader.frag");

    LightingShader.checkCompileErrors(LightingShader.ID, "PROGRAM");
    SkyboxShader.checkCompileErrors(SkyboxShader.ID, "PROGRAM");

    // Define material properties
    Material material;
    material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 32.0f;

    // Load models
    Model GardenPlant("resources/models/AncientEmpire/SM_Env_Garden_Plants_01.obj", "PolygonAncientWorlds_Texture_01_A.png");
    Model Tree("resources/models/AncientEmpire/SM_Env_Tree_Palm_01.obj", "PolygonAncientWorlds_Texture_01_A.png");
    Model Statue("resources/models/AncientEmpire/SM_Prop_Statue_01.obj", "PolygonAncientWorlds_Statue_01.png");

    // Load skybox
    std::vector<std::string> Faces
    {
        "resources/skybox/Corona/Right.png",   // Right
        "resources/skybox/Corona/Left.png",    // Left
        "resources/skybox/Corona/Top.png",     // Top
        "resources/skybox/Corona/Bottom.png",  // Bottom
        "resources/skybox/Corona/Back.png",    // Back
        "resources/skybox/Corona/Front.png"    // Front
    };
    Skybox Skybox(Faces);

    GLightManager.initialize();

    std::cout << "Starting rendering loop..." << std::endl;

    while (!glfwWindowShouldClose(Window))
    {
        float CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        inputManager.processInput(Window, DeltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        checkGLError("After Clear");

        LightingShader.use();
        LightingShader.setMat4("view", GCamera.getViewMatrix());
        LightingShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));
        LightingShader.setVec3("viewPos", GCamera.Position);

        GLightManager.setSpotLightPosition(GCamera.Position);
        GLightManager.setSpotLightDirection(GCamera.Front);

        LightingShader.setVec3("material.ambient", material.ambient);
        LightingShader.setVec3("material.diffuse", material.diffuse);
        LightingShader.setVec3("material.specular", material.specular);
        LightingShader.setFloat("material.shininess", material.shininess);

        GLightManager.updateLighting(LightingShader);

        GLightManager.setDirectionalLightDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
        GLightManager.setDirectionalLightColor(glm::vec3(0.4f, 0.4f, 0.4f));

        auto ModelMatrix = glm::mat4(1.0f);

        for (int X = -5; X <= 5; X++) {
            for (int Z = -5; Z <= 5; Z++) {
                ModelMatrix = glm::mat4(1.0f);
                ModelMatrix = glm::translate(ModelMatrix, glm::vec3(X, -1.0f, Z));
                ModelMatrix = glm::scale(ModelMatrix, glm::vec3(PlantScaleFactor));
                LightingShader.setMat4("model", ModelMatrix);
                GardenPlant.Draw(LightingShader);
            }
        }

        checkGLError("After GardenPlant Draw");

        glm::vec3 TreePositions[] = {
            {-5.0f, -1.0f, -5.0f},
            {5.0f, -1.0f, -5.0f},
        };

        for (glm::vec3 Pos : TreePositions) {
            ModelMatrix = glm::mat4(1.0f);
            ModelMatrix = glm::translate(ModelMatrix, Pos);
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(TreeScaleFactor));
            LightingShader.setMat4("model", ModelMatrix);
            Tree.Draw(LightingShader);
        }

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.0f, 2.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ModelScaleFactor));
        LightingShader.setMat4("model", ModelMatrix);
        Statue.Draw(LightingShader);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
