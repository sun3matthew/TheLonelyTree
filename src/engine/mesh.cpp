#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/mesh.h>
#include <string>

// Maybe look into using pointer..? but tbh you shouldn't be loading meshes during play
Mesh::Mesh(std::vector<Vertex> verticesIn, std::vector<unsigned int> indicesIn, std::vector<Texture> texturesIn)
    : vertices(verticesIn), indices(indicesIn), textures(texturesIn)
{
    setupMesh();
}
Mesh::Mesh(std::vector<Vertex> verticesIn, std::vector<unsigned int> indicesIn)
    : vertices(verticesIn), indices(indicesIn)
{
    textures = {Texture::defaultDiffuse(), Texture::defaultSpecular()};

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> verticesIn, std::vector<Texture> texturesIn)
    : vertices(verticesIn), textures(texturesIn)
{
    std::vector<unsigned int> indices(vertices.size());
    for(int i = 0; i < vertices.size(); i++)
        indices.push_back(i);
    this->indices = indices;

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> verticesIn)
    : vertices(verticesIn)
{
    std::vector<unsigned int> indices(vertices.size());
    for(int i = 0; i < vertices.size(); i++)
        indices.push_back(i);
    this->indices = indices;

    textures = {Texture::defaultDiffuse(), Texture::defaultSpecular()};

    setupMesh();
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
void Mesh::draw(Shader &shader) 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
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

        shader.setInt(("material." + TextureTypeToString(type) + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].getID());
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}  