#pragma once

#include <glew.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include "mesh.h"

class Model
{
public:
    Model(const std::string& modelPath, const std::string& texturePath);
    void Draw(const Shader& shader) const;

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::string texturePath;

    void loadModel(const std::string& path);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};