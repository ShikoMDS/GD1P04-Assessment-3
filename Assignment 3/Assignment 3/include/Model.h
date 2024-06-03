#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

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
    std::vector<Texture> textures_loaded;
    std::string texturePath;

    void loadModel(const std::string& path);
    void processNode();
    Mesh processMesh();
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
