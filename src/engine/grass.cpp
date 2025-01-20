#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/grass.h>
#include <string>

#include <world_generation.h>
#include <noise_generation.h>

Grass::Grass(){
    time = 0;

    float worldSize = (int)WorldGeneration::worldSize();
    float density = 1.5;
    for(float x = 0; x < worldSize; x += density){
        for(float y = 0; y < worldSize; y += density){
            PrimitiveVertex vertex;
            vertex.Position = glm::vec3(x, WorldGeneration::getHeightAt(x, y), y);
            vertices.push_back(vertex);
        }
    }

    perlinLane = NoiseGeneration::GetPerlinLane(83475923u, 512, 1, 0.01f);

    setupMesh();
}

Grass::~Grass(){
    delete perlinLane;
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

    glBindVertexArray(0);
}

void Grass::drawCall(Shader* shader) 
{
    shader->use();
    time += 0.1;
    shader->setFloat("time", time);

    shader->setTexture(perlinLane, 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}  