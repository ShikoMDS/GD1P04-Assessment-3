#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include <glm.hpp>
#include "Shader.h"

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

class LightManager {
public:
    LightManager();

    void initialize();
    void updateLighting(const Shader& shader) const;
    void togglePointLights();
    void toggleDirectionalLight();
    void toggleSpotLight();
    void setDirectionalLightDirection(const glm::vec3& direction);
    void setSpotLightPosition(const glm::vec3& position, const glm::vec3& direction);
    bool arePointLightsOn() const { return pointLightsOn; }

private:
    PointLight pointLights[2];
    DirectionalLight directionalLight;
    SpotLight spotLight;

    bool pointLightsOn;
    bool directionalLightOn;
    bool spotlightOn;
};

#endif // LIGHTMANAGER_H
