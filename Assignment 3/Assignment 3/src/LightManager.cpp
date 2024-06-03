#include "LightManager.h"

LightManager::LightManager() : pointLightsOn(true), directionalLightOn(true), spotLightOn(true) {}

void LightManager::initialize()
{
    pointLights[0] = { glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f, 0.5f, 0.31f), 1.0f, 0.09f, 0.032f };
    pointLights[1] = { glm::vec3(1.2f, -1.0f, 2.0f), glm::vec3(0.5f, 0.5f, 1.0f), 1.0f, 0.09f, 0.032f };
    directionalLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.4f, 0.4f, 0.4f) };
    spotLight = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)) };
}

void LightManager::updateLighting(const Shader& shader) const
{
    for (int i = 0; i < 2; ++i)
    {
        std::string number = std::to_string(i);
        shader.setVec3("pointLights[" + number + "].position", pointLights[i].position);
        shader.setVec3("pointLights[" + number + "].color", pointLights[i].color);
        shader.setFloat("pointLights[" + number + "].constant", pointLights[i].constant);
        shader.setFloat("pointLights[" + number + "].linear", pointLights[i].linear);
        shader.setFloat("pointLights[" + number + "].quadratic", pointLights[i].quadratic);
    }

    shader.setVec3("directionalLight.direction", directionalLight.direction);
    shader.setVec3("directionalLight.color", directionalLight.color);

    shader.setVec3("spotLight.position", spotLight.position);
    shader.setVec3("spotLight.color", spotLight.color);
    shader.setVec3("spotLight.direction", spotLight.direction);
    shader.setFloat("spotLight.cutOff", spotLight.cutOff);
    shader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
}

void LightManager::togglePointLights()
{
    pointLightsOn = !pointLightsOn;
    if (pointLightsOn)
    {
        for (int i = 0; i < 2; ++i)
        {
            pointLights[i].color = glm::vec3(1.0f, 0.5f, 0.31f);
        }
    }
    else
    {
        for (int i = 0; i < 2; ++i)
        {
            pointLights[i].color = glm::vec3(0.0f);
        }
    }
}

void LightManager::toggleDirectionalLight()
{
    directionalLightOn = !directionalLightOn;
    if (directionalLightOn)
    {
        directionalLight.color = glm::vec3(0.4f, 0.4f, 0.4f);
    }
    else
    {
        directionalLight.color = glm::vec3(0.0f);
    }
}

void LightManager::toggleSpotLight()
{
    spotLightOn = !spotLightOn;
    if (spotLightOn)
    {
        spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    else
    {
        spotLight.color = glm::vec3(0.0f);
    }
}
