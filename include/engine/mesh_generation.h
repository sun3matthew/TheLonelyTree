#ifndef MESH_GENERATION_H
#define MESH_GENERATION_H

#include <engine/mesh.h>
#include <vector>
#include <engine/constants.h>
#include <PerlinNoise.hpp>


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

        //TODO Investigate performance on the fact that this is copied.
        return new Mesh("model", std::move(vertices), std::move(indices));
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

        return new Mesh("model", std::move(vertices), std::move(indices));
    }

    static Mesh* Terrain(unsigned int seed, int xSegments, int ySegments){
        const siv::PerlinNoise perlin{ seed };

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (unsigned int y = 0; y <= ySegments; ++y){
            for (unsigned int x = 0; x <= xSegments; ++x){
                float xSegment = (float)x / (float)xSegments;
                float ySegment = (float)y / (float)ySegments;
                float noise = perlin.octave2D_01((x * 0.08), (y * 0.08), 16);

                Vertex vertex;
                vertex.Position = glm::vec3(xSegment - 0.5f, noise, ySegment - 0.5f);
                vertex.Normal = glm::vec3(0, 0, 0);
                vertex.TexCoords = glm::vec2(xSegment, ySegment);
                vertices.push_back(vertex);
            }
        }

        // Calculate normals
        for (unsigned int y = 0; y < ySegments; ++y) {
            for (unsigned int x = 0; x < xSegments; ++x) {
                unsigned int current = y * (xSegments + 1) + x;
                unsigned int right = current + 1;
                unsigned int below = current + (xSegments + 1);
                unsigned int bellowRight = current + (xSegments + 1) + 1;

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

        return new Mesh("model", std::move(vertices), std::move(indices)
            ,std::vector<Texture>{Texture::diffuse(0x80, 0x9D, 0x3C), Texture::specular(0, 0, 0), Texture::defaultGlossy()}
        );
    }
private:
    // Private constructor to prevent instantiation
    MeshGeneration() = delete;
};

#endif // MESH_GENERATION_H
