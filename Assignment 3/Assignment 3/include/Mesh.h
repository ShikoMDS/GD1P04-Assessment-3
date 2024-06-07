#pragma once

#include <glew.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    bool operator==(const Vertex& other) const {
        return Position == other.Position && Normal == other.Normal && TexCoords == other.TexCoords;
    }
};

namespace std {
    template<> struct hash<glm::vec2> {
        size_t operator()(const glm::vec2& v) const {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
        }
    };

    template<> struct hash<glm::vec3> {
        size_t operator()(const glm::vec3& v) const {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2);
        }
    };

    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.Position) ^
                (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.TexCoords) << 1);
        }
    };
}

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(const Shader& shader) const;

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};
