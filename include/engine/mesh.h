#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <engine/shader.h>
#include <engine/texture.h>
#include <engine/render_object.h>

#include <vector>
#include <string>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent = glm::vec3(-2);
    glm::vec3 Bitangent = glm::vec3(-2);
};

class Mesh : public RenderObject{
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        std::vector<Texture>      textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        Mesh(std::vector<Vertex> vertices);
        // TODO add destructor

        void drawCall(Shader* shader) override;
        void updateTexture(Texture);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        std::vector<unsigned int> generateIndices(const std::vector<Vertex>&);
        void ComputeTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        void setupMesh();
};  

#endif