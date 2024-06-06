#pragma once

#include <glew.h>
#include <glm.hpp>
#include "Shader.h"

class LightManager
{
public:
    LightManager();
    void initialize();
    void updateLighting(const Shader& shader) const;
    void togglePointLights();
    void toggleDirectionalLight();
    void toggleSpotLight();

    bool arePointLightsOn() const { return pointLightsOn; }

private:
    struct PointLight
    {
        glm::vec3 position;
        glm::vec3 color;
        float constant;
        float linear;
        float quadratic;
    } pointLights[2];

    struct DirectionalLight
    {
        glm::vec3 direction;
        glm::vec3 color;
    } directionalLight;

    struct SpotLight
    {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;
    } spotLight;

    bool pointLightsOn;
    bool directionalLightOn;
    bool spotLightOn;
};
