#include "Model.h"
#include <iostream>
#include <unordered_map>
#include <fstream>

#include "tiny_obj_loader.h"
#include "stb_image.h"

Model::Model(const std::string& modelPath, const std::string& texturePath) {
    this->directory = "resources/textures"; // Set the directory for textures
    loadModel(modelPath);
    loadTexture(texturePath); // Load texture separately
}

void Model::Draw(const Shader& shader) const {
    for (const auto& mesh : meshes)
        mesh.Draw(shader);
}

void Model::loadModel(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true);

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to load/parse .obj." << std::endl;
        return;
    }

    for (const auto& shape : shapes) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        for (const auto& index : shape.mesh.indices) {
            Vertex vertex = {};

            vertex.Position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.normal_index >= 0) {
                vertex.Normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) {
                vertex.TexCoords = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }

        Mesh mesh(vertices, indices, textures);
        meshes.push_back(mesh);
    }
}

void Model::loadTexture(const std::string& path) {
    std::string fullPath = directory + '/' + path;
    std::cout << "Loading texture: " << fullPath << std::endl;

    Texture texture;
    texture.id = TextureFromFile(fullPath.c_str(), directory);
    texture.type = "texture_diffuse";
    texture.path = path;
    textures_loaded.push_back(texture);

    // Apply the texture to all meshes
    for (auto& mesh : meshes) {
        mesh.textures.push_back(texture);
    }
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) {
    std::string filename = std::string(path);

    std::cout << "Loading texture: " << filename << std::endl;

    // Print the absolute path
    char absPath[1024];
    _fullpath(absPath, filename.c_str(), sizeof(absPath));
    std::cout << "Absolute path: " << absPath << std::endl;

    // Check if file exists
    std::ifstream file(absPath);
    if (!file.good()) {
        std::cerr << "File does not exist: " << absPath << std::endl;
        return 0;
    }
    file.close();

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
