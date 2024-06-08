#pragma once

#include <glew.h>
#include <glm.hpp>
#include <vector>
#include "shader.h"

class Skybox
{
public:
    Skybox(const std::vector<std::string>& faces);
    void Draw(const Shader& shader) const;

private:
    unsigned int VAO, VBO;
    unsigned int cubemapTexture;

    void setupSkybox();
    unsigned int loadCubemap(const std::vector<std::string>& faces);
};
