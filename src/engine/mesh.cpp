#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/mesh.h>
#include <string>

// Maybe look into using pointer..? but tbh you shouldn't be loading meshes during play
Mesh::Mesh(std::string shaderName, 
           std::vector<Vertex> verticesIn, 
           std::vector<unsigned int> indicesIn, 
           std::vector<Texture> texturesIn)
    : RenderObject(shaderName), 
      vertices(std::move(verticesIn)), 
      indices(std::move(indicesIn)), 
      textures(std::move(texturesIn))
{
    setupMesh();
}

Mesh::Mesh(std::string shaderName, 
           std::vector<Vertex> verticesIn, 
           std::vector<unsigned int> indicesIn)
    : Mesh(shaderName, std::move(verticesIn), std::move(indicesIn), 
           {Texture::defaultDiffuse(), Texture::defaultSpecular()})
{}

Mesh::Mesh(std::string shaderName, 
           std::vector<Vertex> verticesIn, 
           std::vector<Texture> texturesIn)
    : Mesh(shaderName, std::move(verticesIn), generateIndices(verticesIn), std::move(texturesIn))
{}

Mesh::Mesh(std::string shaderName, 
           std::vector<Vertex> verticesIn)
    : Mesh(shaderName, std::move(verticesIn), generateIndices(verticesIn), 
           {Texture::defaultDiffuse(), Texture::defaultSpecular(), Texture::defaultGlossy()})
{}

// Helper function to generate default indices
std::vector<unsigned int> Mesh::generateIndices(const std::vector<Vertex>& vertices) {
    std::vector<unsigned int> indices(vertices.size());
    for (unsigned int i = 0; i < vertices.size(); ++i) {
        indices[i] = i;
    }
    return indices;
}


//TODO Encapsulate better
void Mesh::updateTextures(std::vector<Texture> textures){
    this->textures = textures;
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

    glBindVertexArray(0);
}

void Mesh::drawCall(Shader* shader) 
{
    shader->use();

    shader->setMat4("model", glm::value_ptr(modelMatrix));

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int glossyNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        std::string number;
        // std::string name = textures[i].type;

        TextureType type = textures[i].type;
        if(type == TextureType::Diffuse)
            number = std::to_string(diffuseNr++);
        else if(type == TextureType::Specular)
            number = std::to_string(specularNr++);
        else if(type == TextureType::Glossy)
            number = std::to_string(glossyNr++);

        shader->setInt(("material." + TextureTypeToString(type) + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].getID());
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}  