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
};

class Mesh : public RenderObject{
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        std::vector<Texture>      textures;

        std::string shaderName;

        Mesh(std::string shaderName, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::string shaderName, std::vector<Vertex> vertices, std::vector<Texture> textures);
        Mesh(std::string shaderName, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        Mesh(std::string shaderName, std::vector<Vertex> vertices);
        // TODO add destructor

        void drawCall(Shader* shader) override;
        void updateTextures(std::vector<Texture> textures);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        std::vector<unsigned int> generateIndices(const std::vector<Vertex>& vertices);
        void setupMesh();
};  

#endif