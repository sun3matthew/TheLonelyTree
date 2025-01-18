#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/grass.h>
#include <string>

Grass::Grass(){
    // for(int i = 0; i < 10000; i++){
    //     positions.push_back(glm::vec3(i / 100, i % 100, 0));
    // }
    for(int x = 0; x < 4000; x++){
        for(int y = 0; y < 4000; y++){
            PrimitiveVertex vertex;
            vertex.Position = glm::vec3(x * 0.1, 0, y * 0.1);
            vertices.push_back(vertex);
            // positions.push_back(glm::vec3(x * 4, 0, y * 4));
        }
    }

    setupMesh();
}

void Grass::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PrimitiveVertex), &vertices[0], GL_STATIC_DRAW);  

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, Normal));

    // glGenBuffers(1, &instanceVBO);
    // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

    // // Vertex attribute for matrix
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    // glEnableVertexAttribArray(2);
    // glVertexAttribDivisor(2, 1); // Tell OpenGL this is per-instance data

    glBindVertexArray(0);
}

void Grass::drawCall(Shader* shader) 
{
    shader->use();

    glBindVertexArray(VAO);
    // Specify the mode, starting index, vertex count, and instance count
    // glDrawArraysInstanced(GL_POINTS, 0, vertices.size(), positions.size());
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}  