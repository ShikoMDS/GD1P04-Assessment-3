#include "LightManager.h"
#include <iostream>

LightManager::LightManager() : pointLightsOn(true), directionalLightOn(true), spotlightOn(true) {}

void LightManager::initialize()
{
    pointLights[0] = { glm::vec3(3.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f }; // Red
    pointLights[1] = { glm::vec3(-3.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.09f, 0.032f }; // Blue
    directionalLight = { glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) }; // White light

    // Increase the cutOff and outerCutOff angles to make the spotlight radius larger
    spotLight = { glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::cos(glm::radians(20.0f)), glm::cos(glm::radians(25.0f)), 1.0f, 0.09f, 0.032f };
}

void LightManager::updateLighting(const Shader& shader) const
{
    for (int i = 0; i < 2; ++i)
    {
        std::string number = std::to_string(i);
        shader.setVec3("pointLights[" + number + "].position", pointLights[i].position);
        shader.setVec3("pointLights[" + number + "].color", pointLightsOn ? pointLights[i].color : glm::vec3(0.0f));
        shader.setFloat("pointLights[" + number + "].constant", pointLights[i].constant);
        shader.setFloat("pointLights[" + number + "].linear", pointLights[i].linear);
        shader.setFloat("pointLights[" + number + "].quadratic", pointLights[i].quadratic);
    }

    shader.setVec3("dirLight.direction", directionalLight.direction);
    shader.setVec3("dirLight.color", directionalLightOn ? directionalLight.color : glm::vec3(0.0f));

    shader.setVec3("spotLight.position", spotLight.position);
    shader.setVec3("spotLight.direction", spotLight.direction);
    shader.setVec3("spotLight.color", spotlightOn ? spotLight.color : glm::vec3(0.0f));
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
    directionalLight.color = directionalLightOn ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.0f);
    std::cout << "Directional light toggled: " << (directionalLightOn ? "On" : "Off") << std::endl;
}

void LightManager::toggleSpotLight()
{
    spotlightOn = !spotlightOn;
    std::cout << "Spotlight toggled: " << (spotlightOn ? "On" : "Off") << std::endl;
}

void LightManager::setDirectionalLightDirection(const glm::vec3& direction)
{
    directionalLight.direction = direction;
}

void LightManager::setSpotLightPosition(const glm::vec3& position, const glm::vec3& direction)
{
    spotLight.position = position;
    spotLight.direction = direction;
}
