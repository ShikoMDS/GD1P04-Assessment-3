#pragma once

#include <vector>
#include <string>
#include <glew.h>
#include <glm.hpp>
#include "Shader.h"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void Draw(const Shader& shader);

private:
    unsigned int loadCubemap(const std::vector<std::string>& faces);
    void setupSkybox();

    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
};
