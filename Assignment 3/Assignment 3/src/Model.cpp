#include "Model.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
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

    for (const auto& shape : shapes)
    {
        processNode(shape, attrib);
    }
}

void Model::processNode(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib)
{
    Mesh mesh = processMesh(shape, attrib);
    meshes.push_back(mesh);
}

Mesh Model::processMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        Vertex vertex;
        vertex.Position = glm::vec3(attrib.vertices[3 * v + 0], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]);
        vertex.Normal = glm::vec3(attrib.normals[3 * v + 0], attrib.normals[3 * v + 1], attrib.normals[3 * v + 2]);
        vertex.TexCoords = glm::vec2(attrib.texcoords[2 * v + 0], attrib.texcoords[2 * v + 1]);
        vertices.push_back(vertex);
    }

    for (const auto& index : shape.mesh.indices) {
        indices.push_back(index.vertex_index);
    }

    Texture texture(texturePath.c_str(), "resources/textures", "texture_diffuse");
    textures.push_back(texture);

    return Mesh(vertices, indices, textures);
}

unsigned int Model::getTextureID() const {
    if (!meshes.empty() && !meshes[0].textures.empty()) {
        return meshes[0].textures[0].id;
    }
    return 0;
}

unsigned int Model::getSpecularTextureID() const {
    if (meshes.size() > 1 && !meshes[1].textures.empty()) {
        return meshes[1].textures[0].id;
    }
    return 0;
}
