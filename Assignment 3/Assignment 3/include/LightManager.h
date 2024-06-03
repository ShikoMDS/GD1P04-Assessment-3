#pragma once

#include <glew.h>
#include <glm.hpp>
#include "shader.h"

class LightManager
{
public:
    LightManager();
    void initialize();
    void updateLighting(const Shader& shader) const;
    void togglePointLights();
    void toggleDirectionalLight();
    void toggleSpotLight();

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
        glm::vec3 color;
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;
    } spotLight;

    bool pointLightsOn;
    bool directionalLightOn;
    bool spotLightOn;
};
