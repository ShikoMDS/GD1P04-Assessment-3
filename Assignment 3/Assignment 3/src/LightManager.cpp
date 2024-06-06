#include "LightManager.h"
#include <iostream>

LightManager::LightManager() : pointLightsOn(true), directionalLightOn(true), spotLightOn(true) {}

void LightManager::initialize()
{
    pointLights[0] = { glm::vec3(3.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f }; // Red
    pointLights[1] = { glm::vec3(-3.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.09f, 0.032f }; // Blue
    directionalLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.4f, 0.4f, 0.4f) };
    spotLight = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)), 1.0f, 0.09f, 0.032f };
}

void LightManager::updateLighting(const Shader& shader) const
{
    if (pointLightsOn) {
        for (int i = 0; i < 2; ++i)
        {
            std::string number = std::to_string(i);
            shader.setVec3("pointLights[" + number + "].position", pointLights[i].position);
            shader.setVec3("pointLights[" + number + "].color", pointLights[i].color);
            shader.setFloat("pointLights[" + number + "].constant", pointLights[i].constant);
            shader.setFloat("pointLights[" + number + "].linear", pointLights[i].linear);
            shader.setFloat("pointLights[" + number + "].quadratic", pointLights[i].quadratic);
        }
    }
    else {
        for (int i = 0; i < 2; ++i)
        {
            std::string number = std::to_string(i);
            shader.setVec3("pointLights[" + number + "].color", glm::vec3(0.0f)); // Turn off lights
        }
    }

    shader.setVec3("directionalLight.direction", directionalLight.direction);
    shader.setVec3("directionalLight.color", directionalLight.color);

    shader.setVec3("spotLight.position", spotLight.position);
    shader.setVec3("spotLight.direction", spotLight.direction);
    shader.setVec3("spotLight.color", spotLight.color);
    shader.setFloat("spotLight.cutOff", spotLight.cutOff);
    shader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
    shader.setFloat("spotLight.constant", spotLight.constant);
    shader.setFloat("spotLight.linear", spotLight.linear);
    shader.setFloat("spotLight.quadratic", spotLight.quadratic);
}

void LightManager::togglePointLights()
{
    pointLightsOn = !pointLightsOn;
    std::cout << "Point lights toggled: " << (pointLightsOn ? "On" : "Off") << std::endl;
}

void LightManager::toggleDirectionalLight()
{
    directionalLightOn = !directionalLightOn;
    directionalLight.color = directionalLightOn ? glm::vec3(0.4f, 0.4f, 0.4f) : glm::vec3(0.0f);
    std::cout << "Directional light toggled: " << (directionalLightOn ? "On" : "Off") << std::endl;
}

void LightManager::toggleSpotLight()
{
    spotLightOn = !spotLightOn;
    spotLight.color = spotLightOn ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.0f);
    std::cout << "Spot light toggled: " << (spotLightOn ? "On" : "Off") << std::endl;
}
