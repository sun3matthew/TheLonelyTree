#ifndef MESH_GENERATION_H
#define MESH_GENERATION_H

#include <engine/mesh.h>
#include <vector>
#include <engine/constants.h>

class MeshGeneration {
public:
    // Static methods
    static Mesh Sphere(int xSegments, int ySegments){
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
        // return Mesh(std::move(vertices), std::move(Indices));
        return Mesh(vertices, indices);
    }



private:
    // Private constructor to prevent instantiation
    MeshGeneration() = delete;
};

#endif // MESH_GENERATION_H
