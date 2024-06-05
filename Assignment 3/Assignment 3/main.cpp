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
constexpr float MineLightScaleFactor = 0.005f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    inputManager.framebufferSizeCallback(window, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    inputManager.mouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    inputManager.scrollCallback(window, xoffset, yoffset);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << '\n';
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    Shader LightingShader("resources/shaders/BlinnPhong.vert", "resources/shaders/BlinnPhong.frag");
    Shader ReflectionShader("resources/shaders/ReflectionVertexShader.vert", "resources/shaders/ReflectionFragmentShader.frag");
    Shader SkyboxShader("resources/shaders/SkyboxVertexShader.vert", "resources/shaders/SkyboxFragmentShader.frag");
    Shader PointLightShader("resources/shaders/PointLight.vert", "resources/shaders/PointLight.frag");

    Model GardenPlant("resources/models/AncientEmpire/SM_Env_Garden_Plants_01.obj", "PolygonAncientWorlds_Texture_01_A.png");
    Model Tree("resources/models/AncientEmpire/SM_Env_Tree_Palm_01.obj", "PolygonAncientWorlds_Texture_01_A.png");
    Model Statue("resources/models/AncientEmpire/SM_Prop_Statue_01.obj", "PolygonAncientWorlds_Texture_01_A.png");
    Model MineLight("resources/models/SciFiSpace/SM_Prop_Mine_01.obj", "PolygonSciFiSpace_Texture_01_A.png");

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

    GLightManager.initialize();

    // Set shader uniforms
    LightingShader.use();
    LightingShader.setInt("material.diffuse", 0); // Texture unit 0
    LightingShader.setInt("material.specular", 1); // Texture unit 1

    while (!glfwWindowShouldClose(Window))
    {
        float CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        inputManager.processInput(Window, DeltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        LightingShader.use();
        LightingShader.setVec3("viewPos", GCamera.Position);
        LightingShader.setMat4("view", GCamera.getViewMatrix());
        LightingShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));

        // Update spotlight position and direction
        LightingShader.setVec3("lights[1].position", GCamera.Position);
        LightingShader.setVec3("lights[1].direction", GCamera.Front);

        // Set lighting uniforms
        GLightManager.updateLighting(LightingShader);

        auto ModelMatrix = glm::mat4(1.0f);

        // Activate the textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GardenPlant.getTextureID()); // Assuming GardenPlant.getTextureID() returns the correct texture ID

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GardenPlant.getSpecularTextureID()); // Assuming GardenPlant.getSpecularTextureID() returns the correct texture ID

        for (int X = -5; X <= 5; X++) {
            for (int Z = -5; Z <= 5; Z++) {
                ModelMatrix = glm::mat4(1.0f);
                ModelMatrix = glm::translate(ModelMatrix, glm::vec3(X, -1.0f, Z));
                ModelMatrix = glm::scale(ModelMatrix, glm::vec3(PlantScaleFactor));
                LightingShader.setMat4("model", ModelMatrix);
                GardenPlant.Draw(LightingShader);
            }
        }

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

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ModelScaleFactor));
        LightingShader.setMat4("model", ModelMatrix);
        Statue.Draw(LightingShader);

        PointLightShader.use();
        PointLightShader.setMat4("view", GCamera.getViewMatrix());
        PointLightShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));

        glm::vec3 PointLightPositions[] = {
            { 1.0f, 0.5f, 1.0f },
            { -1.0f, 0.5f, -1.0f }
        };
        glm::vec3 PointLightColors[] = {
            {1.0f, 0.0f, 0.0f}, // Red light
            {0.0f, 0.0f, 1.0f}  // Blue light
        };

        for (int i = 0; i < 2; ++i) {
            glm::vec3 Pos = PointLightPositions[i];
            glm::vec3 Color = PointLightColors[i];
            ModelMatrix = glm::mat4(1.0f);
            ModelMatrix = glm::translate(ModelMatrix, Pos);
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(MineLightScaleFactor));
            PointLightShader.setMat4("model", ModelMatrix);
            PointLightShader.setVec3("lightColor", Color);
            MineLight.Draw(PointLightShader);
        }

        ReflectionShader.use();
        ReflectionShader.setMat4("view", GCamera.getViewMatrix());
        ReflectionShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));
        ReflectionShader.setVec3("cameraPos", GCamera.Position);
        ReflectionShader.setMat4("model", ModelMatrix);
        ReflectionShader.setBool("useTexture", true);
        ReflectionShader.setInt("skybox", 0);
        // Statue.Draw(ReflectionShader); // Commenting this out to remove extra statue rendering

        glDepthFunc(GL_LEQUAL);
        SkyboxShader.use();
        SkyboxShader.setMat4("view", glm::mat4(glm::mat3(GCamera.getViewMatrix())));
        SkyboxShader.setMat4("projection", GCamera.getProjectionMatrix(ScrWidth, ScrHeight));
        Skybox.Draw(SkyboxShader);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
