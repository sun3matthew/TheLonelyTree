
#include <world_generation.h>
#include <vector>
#include <iostream>

unsigned int WorldGeneration::seed = 0;
siv::PerlinNoise WorldGeneration::seededPerlin;
float WorldGeneration::height = 0;
float WorldGeneration::size = 0;

float WorldGeneration::getHeightAt(float x, float y){
    float noise = seededPerlin.octave2D_01((x * 0.003), (y * 0.003), 16);
    return (noise - 1) * height;
}

Mesh* WorldGeneration::createWorld(unsigned int seedIn, float heightIn, float sizeIn, float density){
    seededPerlin = siv::PerlinNoise{seedIn};

    seed = seedIn;
    height = heightIn;
    size = sizeIn;

    int segments = -1; //! I should calculate this but this method should never fail.

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (float y = 0; y <= size; y += density){
        segments++; // Lmao
        for (float x = 0; x <= size; x += density){
            Vertex vertex;
            vertex.Position = glm::vec3(x, getHeightAt(x, y), y);
            vertex.Normal = glm::vec3(0, 0, 0);
            vertex.TexCoords = glm::vec2(x/size, y / size);
            vertices.push_back(vertex);
        }
    }

    // Calculate normals
    for (unsigned int y = 0; y < segments; ++y) {
        for (unsigned int x = 0; x < segments; ++x) {
            unsigned int current = y * (segments + 1) + x;
            unsigned int right = current + 1;
            unsigned int below = current + (segments + 1);
            unsigned int bellowRight = current + (segments + 1) + 1;

            glm::vec3 p0 = vertices[current].Position;
            glm::vec3 p1 = vertices[right].Position;
            glm::vec3 p2 = vertices[below].Position;
            glm::vec3 p3 = vertices[bellowRight].Position;


            glm::vec3 v1 = p1 - p0;
            glm::vec3 v2 = p2 - p0;

            glm::vec3 v3 = p2 - p3;
            glm::vec3 v4 = p1 - p3;

            glm::vec3 normal = glm::normalize(glm::cross(v2, v1)) + glm::normalize(glm::cross(v4, v3));

            // Accumulate normals for each vertex
            vertices[current].Normal += normal;
            vertices[right].Normal += normal;
            vertices[below].Normal += normal;
        }
    }

    // Normalize all normals
    for (auto &vertex : vertices) {
        vertex.Normal = glm::normalize(vertex.Normal);
    }

    //Just, draw it out
    for (unsigned int y = 0; y < segments; ++y){
        for (unsigned int x = 0; x < segments; ++x){
            indices.push_back(y       * (segments + 1) + x + 1);
            indices.push_back(y       * (segments + 1) + x);
            indices.push_back((y + 1) * (segments + 1) + x);

            indices.push_back((y + 1) * (segments + 1) + x);
            indices.push_back((y + 1) * (segments + 1) + x + 1);
            indices.push_back(y       * (segments + 1) + x + 1);
        }
    }

    // std::cout << vertices.size() << std::endl;
    // std::cout << indices.size() << std::endl;
    // std::cout << segments << std::endl;

    return new Mesh(std::move(vertices), std::move(indices)
        ,std::vector<Texture>{Texture::diffuse(0x60, 0x5D, 0x53), Texture::specular(0, 0, 0), Texture::defaultGlossy()}
    );
}

float WorldGeneration::worldSize(){
    return size;
}
