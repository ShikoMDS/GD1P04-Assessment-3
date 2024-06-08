#pragma once

#include "Shader.h"
#include <glm.hpp>
#include <string>

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float ambientStrength; // Add this line
};


struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

class LightManager
{
public:
    LightManager();

    void initialize();
    void updateLighting(const Shader& shader) const;

    void togglePointLights();
    void toggleDirectionalLight();
    void toggleSpotLight();

    void setDirectionalLightDirection(const glm::vec3& direction);
    void setDirectionalLightColor(const glm::vec3& color);

    void setSpotLightPosition(const glm::vec3& position);
    void setSpotLightDirection(const glm::vec3& direction);
    SpotLight getSpotLight() const;

    bool isPointLightsOn() const { return pointLightsOn; }
    const PointLight& getPointLight(int index) const { return pointLights[index]; }

private:
    PointLight pointLights[2];
    DirectionalLight directionalLight;
    SpotLight spotLight;

    bool pointLightsOn;
    bool directionalLightOn;
    bool spotLightOn;
};
