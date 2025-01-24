#include <glad/glad.h> // holds all OpenGL type declarations

#include <engine/grass.h>
#include <string>

#include <world_generation.h>
#include <noise_generation.h>

#include <engine/constants.h>
#include <engine/glfw_wrapper.h>
#include <iostream>

Grass::Grass(){
    time = 0;

    float worldSize = (int)WorldGeneration::worldSize();
    float density = 3.0;

    float center = worldSize/2;

    float distToOuter = 100;
    for(float r = 0.1; r < worldSize/2 * 0.99; r += (r + 500) / 120){
        float rInc = (r + 500) / 80;
        float offset = r / 100;
        for(float t = 0; t <= 1; t += 0.004 / pow(r,0.4)){
            float randR = r + sin(t * 10000) * rInc;
            float x = randR * cos(t * TAU + offset) + center;
            float y = randR * sin(t * TAU + offset) + center;

            PrimitiveVertex vertex;
            vertex.Position = glm::vec3(x, WorldGeneration::getHeightAt(x, y), y);
            vertices.push_back(vertex);
        }
    }

    float rInc = 0.1;
    for(float r = 0.1; r < worldSize/2 * 0.3; r += (r + 1000) / 100){
        rInc = r / 1000;
        if (rInc < 0.1)
            rInc = 0.1;
        float offset = r / 100;
        for(float t = 0; t <= 1; t += 0.01 / pow(r,0.4)){
            float x = r * cos(t * TAU + offset) + center;
            float y = r * sin(t * TAU + offset) + center;

            PrimitiveVertex vertex;
            vertex.Position = glm::vec3(x, WorldGeneration::getHeightAt(x, y), y);
            vertices.push_back(vertex);
        }
    }


    // generate textures

    // int horizontalResolution = 16;

    // int vainSize = horizontalResolution / 6;
    // std::vector<unsigned char> specularMap;
    // unsigned char specularBase = 0x92;
    // unsigned char specularHighlight = 0x32;
    // for(int i = 0; i < horizontalResolution; i++){
    //     if(abs(horizontalResolution/2 - i) < vainSize/2)
    //         specularMap.emplace_back(specularHighlight);
    //     else
    //         specularMap.emplace_back(specularBase);
    // }

    // for(float x = 0; x < worldSize; x += density){
    //     for(float y = 0; y < worldSize; y += density){
    //         PrimitiveVertex vertex;
    //         vertex.Position = glm::vec3(x, WorldGeneration::getHeightAt(x, y), y);
    //         vertices.push_back(vertex);
    //     }
    // }

    std::cout << vertices.size() << std::endl;

    perlinLane = NoiseGeneration::GetPerlinLane(83475923u, 512 * 2, 1, 0.005f);

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
    time += GLFWWrapper::instance->getDeltaTime() * 4;
    shader->setFloat("time", time);
    shader->setVec3("worldCenter", WorldGeneration::worldSize()/2.0, 0.0, WorldGeneration::worldSize()/2.0);

    shader->setTexture(perlinLane, 0);
    shader->setTexture(GLFWWrapper::instance->getShadowMapPtr(), 1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}  