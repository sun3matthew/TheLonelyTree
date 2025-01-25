#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/mesh.h>
#include <string>

#include <engine/glfw_wrapper.h>

// #include <iostream>
// Maybe look into using pointer..? but tbh you shouldn't be loading meshes during play
Mesh::Mesh(std::vector<Vertex> verticesIn, 
           std::vector<unsigned int> indicesIn, 
           std::vector<Texture> texturesIn)
    : vertices(std::move(verticesIn)), 
      indices(std::move(indicesIn)), 
      textures(std::move(texturesIn))
{
    if(indices.size() == 0)
        indices = generateIndices(vertices);
    // if(vertices[0].Tangent.x < -1)
    //     ComputeTangents(vertices, indices);

    // for(Vertex vertex : vertices){
    //     std::cout << vertex.Normal.x << "," << vertex.Normal.y << "," << vertex.Normal.z << std::endl;
    //     std::cout << vertex.Tangent.x << "," << vertex.Tangent.y << "," << vertex.Tangent.z << std::endl;
    //     std::cout << vertex.Bitangent.x << "," << vertex.Bitangent.y << "," << vertex.Bitangent.z << std::endl;
    //     std::cout << std::endl;
    // }
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> verticesIn, 
           std::vector<unsigned int> indicesIn)
    : Mesh(std::move(verticesIn), std::move(indicesIn), {})
{}

Mesh::Mesh(std::vector<Vertex> verticesIn, 
           std::vector<Texture> texturesIn)
    : Mesh(std::move(verticesIn), {}, std::move(texturesIn))
{}

Mesh::Mesh(std::vector<Vertex> verticesIn)
    : Mesh(std::move(verticesIn), {}, {})
{}

void Mesh::ComputeTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    // Initialize tangents and bitangents
    for (auto& vertex : vertices) {
        vertex.Tangent = glm::vec3(0.0f);
        vertex.Bitangent = glm::vec3(0.0f);
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.Position - v0.Position;
        glm::vec3 edge2 = v2.Position - v0.Position;

        glm::vec2 deltaUV1 = v1.TexCoords - v0.TexCoords;
        glm::vec2 deltaUV2 = v2.TexCoords - v0.TexCoords;

        float det = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
        float f = (det == 0.0f) ? 0.0f : 1.0f / det;

        glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        glm::vec3 bitangent = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

        v0.Tangent += tangent;
        v1.Tangent += tangent;
        v2.Tangent += tangent;

        v0.Bitangent += bitangent;
        v1.Bitangent += bitangent;
        v2.Bitangent += bitangent;
    }

    for (auto& vertex : vertices) {
        vertex.Tangent = glm::normalize(vertex.Tangent);
        vertex.Bitangent = glm::normalize(vertex.Bitangent);
    }
}

// Helper function to generate default indices
std::vector<unsigned int> Mesh::generateIndices(const std::vector<Vertex>& vertices) {
    std::vector<unsigned int> indices(vertices.size());
    for (unsigned int i = 0; i < vertices.size(); ++i) {
        indices[i] = i;
    }
    return indices;
}

void Mesh::updateTexture(Texture newTexture){
    for(int i = 0; i < textures.size(); i++){
        if(textures[i].type == newTexture.type){
            textures[i] = newTexture;
            return;
        }
    }
    textures.push_back(newTexture);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(4);	
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

void Mesh::drawCall(Shader* shader) 
{
    shader->use();

    if (shader->canAcceptAttribute("model")){
        shader->setMat4("model", modelMatrix);
    }

    if (shader->canAcceptAttribute("meshTextures"))
        for(unsigned int i = 0; i < textures.size(); i++)
            shader->setTexture(&textures[i], i);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}  