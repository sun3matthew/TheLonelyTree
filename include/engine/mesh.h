#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <engine/shader.h>
#include <engine/texture.h>

#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        float shininess;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        Mesh(std::vector<Vertex> vertices);
        void draw(Shader &shader);
        void updateTextures(std::vector<Texture> textures);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  

#endif