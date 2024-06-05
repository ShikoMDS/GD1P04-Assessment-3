#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <tiny_obj_loader.h>
#include <vector>
#include "Mesh.h"
#include "Shader.h"

class Model {
public:
    Model(const std::string& modelPath, const std::string& texturePath);
    void Draw(const Shader& shader) const;
    unsigned int getTextureID() const;
    unsigned int getSpecularTextureID() const;

private:
    std::vector<Mesh> meshes;
    std::string texturePath;

    void loadModel(const std::string& path);
    void processNode(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib);
    Mesh processMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib);
};

#endif // MODEL_H
