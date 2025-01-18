#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/grass.h>
#include <string>

#include <PerlinNoise.hpp>

Grass::Grass(){
    time = 0;
    // for(int i = 0; i < 10000; i++){
    //     positions.push_back(glm::vec3(i / 100, i % 100, 0));
    // }

    const siv::PerlinNoise perlin{ 12923952u };

    // for(int x = 0; x < 1000; x++){
    //     float sampleX = (x / 1000.0) * 256;
    //     for(int y = 0; y < 1000; y++){
    //         float sampleY = (y / 1000.0) * 256;
    //         float noise = perlin.octave2D_01((x * 0.08), (y * 0.08), 16);

    //         PrimitiveVertex vertex;
    //         vertex.Position = glm::vec3(x * 1.0, (noise * 50) - 50, y * 1.0);
    //         vertices.push_back(vertex);
    //         // positions.push_back(glm::vec3(x * 4, 0, y * 4));
    //     }
    // }
    for(int x = 0; x < 1000; x++){
        float sampleX = x / 1000.0f * 256;
        for(int y = 0; y < 1000; y++){
            float sampleY = y / 1000.0f * 256;
            float noise = perlin.octave2D_01((sampleX * 0.02), (sampleY * 0.02), 16);

            PrimitiveVertex vertex;
            // vertex.Position = glm::vec3((x / 256.0) * 1000, (noise * 50) - 50, (y / 256.0) * 1000);
            // vertex.Position = glm::vec3((x / 256.0) * 1000, 0, (y / 256.0) * 1000);
            vertex.Position = glm::vec3(x/2.0, (noise * 50) - 50, y/2.0);
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
    time += 0.1;
    shader->setFloat("time", time);

    glBindVertexArray(VAO);
    // Specify the mode, starting index, vertex count, and instance count
    // glDrawArraysInstanced(GL_POINTS, 0, vertices.size(), positions.size());
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}  