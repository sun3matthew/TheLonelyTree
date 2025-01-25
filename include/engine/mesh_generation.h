#ifndef MESH_GENERATION_H
#define MESH_GENERATION_H

#include <engine/mesh.h>
#include <vector>
#include <engine/constants.h>
#include <engine/mesh_generation_buffers.h>


class MeshGeneration {
public:
    // Static methods
    static Mesh* Sphere(int xSegments, int ySegments){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (unsigned int y = 0; y <= ySegments; ++y)
        {
            for (unsigned int x = 0; x <= xSegments; ++x)
            {
                float xSegment = (float)x / (float)xSegments;
                float ySegment = (float)y / (float)ySegments;
                float xPos = std::cos(xSegment * TAU) * std::sin(ySegment * PI); // TAU is 2PI
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * TAU) * std::sin(ySegment * PI);

                Vertex vertex;
                vertex.Position = glm::vec3(xPos, yPos, zPos);
                vertex.Normal = glm::vec3(xPos, yPos, zPos);
                vertex.TexCoords = glm::vec2(xSegment, ySegment);
                vertices.push_back(vertex);
            }
        }

        for (int y = 0; y < ySegments; ++y)
        {
            for (int x = 0; x < xSegments; ++x)
            {
                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back(y       * (xSegments + 1) + x);
                indices.push_back(y       * (xSegments + 1) + x + 1);

                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back(y       * (xSegments + 1) + x + 1);
                indices.push_back((y + 1) * (xSegments + 1) + x + 1);
            }
        }

        return new Mesh(std::move(vertices), std::move(indices), Texture::defaultTextures());
    }

    static Mesh* Cube(){
        std::vector<Vertex> vertices;
        std::vector<float> cubeData = MeshGenerationBuffers::cube();

        for(int i = 0; i < cubeData.size(); i += 8){
            Vertex vertex;
            vertex.Position = glm::vec3(cubeData[i], cubeData[i + 1], cubeData[i + 2]);
            vertex.Normal = glm::vec3(cubeData[i + 3], cubeData[i + 4], cubeData[i + 5]);
            vertex.TexCoords = glm::vec2(cubeData[i + 6], cubeData[i + 7]);
            vertices.push_back(vertex);
        }

        return new Mesh(std::move(vertices), Texture::defaultTextures());
    }

    static Mesh* SkyMap(){
        std::vector<Vertex> vertices;
        std::vector<float> cubeData = MeshGenerationBuffers::skyMap();

        for(int i = 0; i < cubeData.size(); i += 3){
            Vertex vertex;
            vertex.Position = glm::vec3(cubeData[i], cubeData[i + 1], cubeData[i + 2]);
            vertices.push_back(vertex);
        }

        return new Mesh(std::move(vertices));
    }

    static Mesh* Plane(int xSegments, int ySegments){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (unsigned int y = 0; y <= ySegments; ++y){
            for (unsigned int x = 0; x <= xSegments; ++x){
                float xSegment = (float)x / (float)xSegments;
                float ySegment = (float)y / (float)ySegments;

                Vertex vertex;
                vertex.Position = glm::vec3(xSegment - 0.5f, 0, ySegment - 0.5f);
                vertex.Normal = glm::vec3(0, 1, 0);
                vertex.TexCoords = glm::vec2(xSegment, ySegment);
                vertices.push_back(vertex);
            }
        }

        //Just, draw it out
        for (unsigned int y = 0; y < ySegments; ++y){
            for (unsigned int x = 0; x < xSegments; ++x){
                indices.push_back(y       * (xSegments + 1) + x + 1);
                indices.push_back(y       * (xSegments + 1) + x);
                indices.push_back((y + 1) * (xSegments + 1) + x);

                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back((y + 1) * (xSegments + 1) + x + 1);
                indices.push_back(y       * (xSegments + 1) + x + 1);
            }
        }

        return new Mesh(std::move(vertices), std::move(indices), Texture::defaultTextures());
    }

    static Mesh* ScreenQuad(){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        vertices.push_back({glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(1, -1, 0), glm::vec3(0, 0, 1), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(-1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 1)});
        indices = {0, 1, 2, 0, 2, 3};

        return new Mesh(std::move(vertices), std::move(indices), {});
    }
private:
    // Private constructor to prevent instantiation
    MeshGeneration() = delete;
};

#endif // MESH_GENERATION_H
