#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm.hpp>
#include <glew.h>
#include "Shader.h"
#include "Texture.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(const Shader& shader) const;

private:
    // Render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif // MESH_H
