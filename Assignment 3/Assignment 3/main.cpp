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
Camera GCamera(glm::vec3(0.0f, 0.0f, 5.0f));

// Timing
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

// Lighting
LightManager GLightManager;

// Input Manager
InputManager inputManager(GCamera, GLightManager);

// Scale factors for the models
constexpr float ModelScaleFactor = 0.01f;
constexpr float PlantScaleFactor = 0.005f;
constexpr float MineScaleFactor = 0.0025f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    inputManager.framebufferSizeCallback(window, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    inputManager.mouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    inputManager.scrollCallback(window, xoffset, yoffset);
}

// Function to convert degrees to radians
constexpr float DegreesToRadians(float degrees) {
    return degrees * glm::pi<float>() / 180.0f;
}

int main()
{
    std::cout << "Initializing GLFW..." << std::endl;
    // Initialize and configure GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << '\n';
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* Window = glfwCreateWindow(ScrWidth, ScrHeight, "OpenGL Demo", nullptr, nullptr);
    if (!Window) {
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

    std::cout << "Initializing GLEW..." << std::endl;
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << '\n';
        return -1;
    }

    std::cout << "Configuring global OpenGL state..." << std::endl;
    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    std::cout << "Building and compiling shaders..." << std::endl;
    // Build and compile shaders
    Shader LightingShader("resources/shaders/PointLightVertexShader.vert", "resources/shaders/PointLightFragmentShader.frag");
    Shader SimpleShader("resources/shaders/SimpleVertexShader.vert", "resources/shaders/SimpleFragmentShader.frag");
    Shader ReflectionShader("resources/shaders/ReflectionVertexShader.vert", "resources/shaders/ReflectionFragmentShader.frag");
    Shader SkyboxShader("resources/shaders/SkyboxVertexShader.vert", "resources/shaders/SkyboxFragmentShader.frag");

    std::cout << "Loading models..." << std::endl;
    // Load models
    Model GardenPlant("resources/models/AncientEmpire/SM_Env_Garden_Plants_01.obj", "resources/textures/PolygonAncientWorlds_Texture_01_A.png");
    Model Tree("resources/models/AncientEmpire/SM_Env_Tree_Palm_01.obj", "resources/textures/PolygonAncientWorlds_Texture_01_A.png");
    Model Statue("resources/models/AncientEmpire/SM_Prop_Statue_01.obj", "resources/textures/PolygonAncientWorlds_Texture_01_A.png");
    Model Mine("resources/models/SciFiSpace/SM_Prop_Mine_01.obj", "resources/textures/PolygonSciFiSpace_Texture_01_A.png");

    std::cout << "Loading skybox..." << std::endl;
    // Load skybox
    std::vector<std::string> Faces
    {
        "resources/skybox/Corona/Right.png",
        "resources/skybox/Corona/Left.png",
        "resources/skybox/Corona/Top.png",
        "resources/skybox/Corona/Bottom.png",
        "resources/skybox/Corona/Back.png",
        "resources/skybox/Corona/Front.png"
    };
    Skybox Skybox(Faces);

    std::cout << "Initializing lighting..." << std::endl;
    // Initialize lighting
    GLightManager.initialize();

    // Define point light positions and colors using the mines
    glm::vec3 pointLightPositions[] = {
        glm::vec3(3.0f, 2.0f, 0.0f),
        glm::vec3(-3.0f, 2.0f, 0.0f)
    };
    glm::vec3 pointLightColors[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };

    std::cout << "Entering render loop..." << std::endl;
    // Rendering loop
    while (!glfwWindowShouldClose(Window)) {
        // Per-frame time logic
        float CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        // Input
        inputManager.processInput(Window, DeltaTime);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set shader and pass uniforms
        LightingShader.use();
        LightingShader.setMat4("view", GCamera.getViewMatrix());
        LightingShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));
        LightingShader.setVec3("viewPos", GCamera.Position);

        // Set point light uniforms using mines
        GLightManager.updateLighting(LightingShader);

        // Render garden plants as ground
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

        // Render trees around the scene
        glm::vec3 TreePositions[] = {
            {-5.0f, -1.0f, -5.0f}, {5.0f, -1.0f, -5.0f},
            {-5.0f, -1.0f, 5.0f}, {5.0f, -1.0f, 5.0f}
        };

        for (glm::vec3 Pos : TreePositions) {
            ModelMatrix = glm::mat4(1.0f);
            ModelMatrix = glm::translate(ModelMatrix, Pos);
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ModelScaleFactor));
            LightingShader.setMat4("model", ModelMatrix);
            Tree.Draw(LightingShader);
        }

        // Render statue in the middle
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ModelScaleFactor));
        LightingShader.setMat4("model", ModelMatrix);
        Statue.Draw(LightingShader);

        // Render mines as point light sources
        SimpleShader.use();
        SimpleShader.setMat4("view", GCamera.getViewMatrix());
        SimpleShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));

        for (int i = 0; i < 2; ++i) {
            ModelMatrix = glm::mat4(1.0f);
            ModelMatrix = glm::translate(ModelMatrix, pointLightPositions[i]);
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(MineScaleFactor));
            SimpleShader.setMat4("model", ModelMatrix);
            SimpleShader.setVec3("color", pointLightColors[i]);
            Mine.Draw(SimpleShader);
        }

        // Render the reflective model
        ReflectionShader.use();
        ReflectionShader.setMat4("view", GCamera.getViewMatrix());
        ReflectionShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));
        ReflectionShader.setVec3("cameraPos", GCamera.Position);
        ReflectionShader.setMat4("model", ModelMatrix);
        ReflectionShader.setBool("useTexture", true);
        ReflectionShader.setInt("skybox", 0);
        Statue.Draw(ReflectionShader);

        // Render skybox
        glDepthFunc(GL_LEQUAL);
        SkyboxShader.use();
        SkyboxShader.setMat4("view", glm::mat4(glm::mat3(GCamera.getViewMatrix())));
        SkyboxShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));
        Skybox.Draw(SkyboxShader);
        glDepthFunc(GL_LESS);

        // Swap buffers and poll IO events
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}
