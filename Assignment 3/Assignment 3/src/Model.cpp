#include "Model.h"
#include <iostream>

#include "tiny_obj_loader.h"
#include "stb_image.h"

Model::Model(const std::string& modelPath, const std::string& texturePath)
{
    this->texturePath = texturePath;
    loadModel(modelPath);
}

void Model::Draw(const Shader& shader) const
{
    for (const auto& mesh : meshes)
        mesh.Draw(shader);
}

void Model::loadModel(const std::string& path)
{
    // TinyObjLoader code to load model
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

    // Process shapes
    for (const auto& shape : shapes)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Process vertices
        for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
            Vertex vertex;
            vertex.Position = glm::vec3(attrib.vertices[3 * v + 0], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]);
            vertex.Normal = glm::vec3(attrib.normals[3 * v + 0], attrib.normals[3 * v + 1], attrib.normals[3 * v + 2]);
            vertex.TexCoords = glm::vec2(attrib.texcoords[2 * v + 0], attrib.texcoords[2 * v + 1]);
            vertices.push_back(vertex);
        }

        // Process indices
        for (const auto& index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);
        }

        // Load texture
        Texture texture;
        texture.id = TextureFromFile(texturePath.c_str(), directory);
        texture.type = "texture_diffuse";
        texture.path = texturePath;
        textures.push_back(texture);

        // Create mesh and add to meshes vector
        Mesh mesh(vertices, indices, textures);
        meshes.push_back(mesh);
    }
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
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
    else
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
