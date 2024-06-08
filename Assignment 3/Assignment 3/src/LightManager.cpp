#include "LightManager.h"

LightManager::LightManager() : pointLightsOn(true), directionalLightOn(true), spotLightOn(true) {}

void LightManager::initialize()
{
    pointLights[0] = { glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f };
    pointLights[1] = { glm::vec3(1.2f, -1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.09f, 0.032f };
    directionalLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.4f, 0.4f, 0.4f), 0.1f };
    spotLight = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::cos(glm::radians(17.5f)), glm::cos(glm::radians(20.0f)), 1.0f, 0.09f, 0.032f }; // Ensure the color is non-zero
}

void LightManager::updateLighting(const Shader& shader) const
{
    shader.setVec3("directionalLight.direction", directionalLight.direction);
    shader.setVec3("directionalLight.color", directionalLight.color);
    shader.setFloat("directionalLight.ambientStrength", directionalLight.ambientStrength);

    for (int i = 0; i < 2; i++) {
        shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
        shader.setVec3("pointLights[" + std::to_string(i) + "].color", pointLights[i].color);
        shader.setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
        shader.setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
        shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
    }

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
    if (pointLightsOn)
    {
        pointLights[0].color = glm::vec3(1.0f, 0.5f, 0.31f);
        pointLights[1].color = glm::vec3(0.5f, 0.5f, 1.0f);
    }
    else
    {
        pointLights[0].color = glm::vec3(0.0f);
        pointLights[1].color = glm::vec3(0.0f);
    }
}

void LightManager::toggleDirectionalLight()
{
    directionalLightOn = !directionalLightOn;
    directionalLight.color = directionalLightOn ? glm::vec3(0.4f, 0.4f, 0.4f) : glm::vec3(0.0f);
}

void LightManager::toggleSpotLight()
{
    spotLightOn = !spotLightOn;
    spotLight.color = spotLightOn ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.0f);
}

void LightManager::setSpotLightPosition(const glm::vec3& position)
{
    spotLight.position = position;
}

void LightManager::setSpotLightDirection(const glm::vec3& direction)
{
    spotLight.direction = direction;
}

SpotLight LightManager::getSpotLight() const
{
    return spotLight;
}
