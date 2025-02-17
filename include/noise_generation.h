#ifndef NOISE_GENERATION_H
#define NOISE_GENERATION_H

#include <vector>
#include <PerlinNoise.hpp>
#include <engine/texture.h>

#include <cmath>
#include <random>
    
#include <glm/glm.hpp>

#include <iostream>

class NoiseGeneration {
public:
    static Texture* GetPerlinLane(unsigned int seed, int resolution, float aspectRatio, float zoom){
        siv::PerlinNoise seededPerlin{seed};

        int height = resolution;
        int width = (int)(height * aspectRatio);
        unsigned char data[width * height];

        int i = 0;
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                    data[i] = (unsigned char)(0xff * seededPerlin.octave2D_01(x * zoom, y * zoom, 1));
                    i++;
            }
        }

        return new Texture(data, width, height, 1, TextureType::PerlinLane);
    }

    static Texture GetCloudNoise(unsigned int seed, int resolution, float zoom){
        siv::PerlinNoise seededPerlin{seed};

        // unsigned char* perlinData = TDPerlinNoise(seed, resolution, zoom);
        unsigned char* worleyData = TDWorleyNoise(seed, resolution, 20.0f);
        unsigned char* worleyData2 = TDWorleyNoise(seed + 4, resolution, 10.0f);
        unsigned char* worleyData3 = TDWorleyNoise(seed + 2, resolution, 5.0f);

        unsigned char data[resolution * resolution * resolution];

        // copy perlin noise to 3D texture
        int i = 0;
        for(int z = 0; z < resolution; z++){
            for(int y = 0; y < resolution; y++){
                for(int x = 0; x < resolution; x++){
                    // data[i] = (worleyData[i] + perlinData[i]) / 2;
                    // data[i] = (0xff - worleyData[i] + perlinData[i]) / 2;
                    // data[i] = (worleyData[i] + worleyData2[i]) / 2;
                    float r = 1.0f - worleyData[i] / 255.0f;
                    float g = 1.0f - worleyData2[i] / 255.0f;
                    float b = 1.0f - worleyData3[i] / 255.0f;

                    data[i] = (unsigned char)(0xff * (r * 0.5f + g * 0.3f + b * 0.2f));
                    i++;
                }
            }
        }

        // delete[] perlinData;
        delete[] worleyData;
        delete[] worleyData2;
        delete[] worleyData3;

        return Texture(data, resolution, resolution, resolution, 1, TextureType::TDNoise);
    }

    static unsigned char* TDPerlinNoise(unsigned int seed, int resolution, float zoom){
        siv::PerlinNoise seededPerlin{seed};

        int height = resolution;
        int width = resolution;
        int depth = resolution;
        unsigned char* data = new unsigned char[width * height * depth];

        int i = 0;
        for (int z = 0; z < depth; z++){
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    data[i] = static_cast<unsigned char>(0xff * seededPerlin.octave3D_01(x * zoom, y * zoom, z * zoom, 2));
                    i++;
                }
            }
        }

        return data;
    }
    
    static unsigned char* TDWorleyNoise(unsigned int seed, int resolution, float cellSize) {
        int width = resolution;
        int height = resolution;
        int depth = resolution;
        unsigned char* data = new unsigned char[width * height * depth];
    
        std::mt19937 rng(seed);
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
        int gridSize = static_cast<int>(std::ceil(resolution / cellSize));
    
        std::vector<std::vector<std::vector<glm::vec3>>> featurePoints(gridSize,
            std::vector<std::vector<glm::vec3>>(gridSize,
                std::vector<glm::vec3>(gridSize)));
    
        // Generate feature points
        for (int gx = 0; gx < gridSize; gx++) {
            for (int gy = 0; gy < gridSize; gy++) {
                for (int gz = 0; gz < gridSize; gz++) {
                    featurePoints[gx][gy][gz] = glm::vec3(
                        (gx + dist(rng)) * cellSize,
                        (gy + dist(rng)) * cellSize,
                        (gz + dist(rng)) * cellSize
                    );
                }
            }
        }
    
        int i = 0;
        for (int z = 0; z < depth; z++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float minDist = 1e10f;
                    glm::vec3 voxelPos(x, y, z);
    
                    int gx = static_cast<int>(x / cellSize);
                    int gy = static_cast<int>(y / cellSize);
                    int gz = static_cast<int>(z / cellSize);
    
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            for (int dz = -1; dz <= 1; dz++) {
                                int neighborX = (gx + dx + gridSize) % gridSize;
                                int neighborY = (gy + dy + gridSize) % gridSize;
                                int neighborZ = (gz + dz + gridSize) % gridSize;
    
                                glm::vec3 point = featurePoints[neighborX][neighborY][neighborZ];
    
                                // Compute wrapped distance
                                glm::vec3 wrappedDist = voxelPos - point;
                                wrappedDist.x -= resolution * std::round(wrappedDist.x / resolution);
                                wrappedDist.y -= resolution * std::round(wrappedDist.y / resolution);
                                wrappedDist.z -= resolution * std::round(wrappedDist.z / resolution);
    
                                float dist = glm::length(wrappedDist);
    
                                if (dist < minDist) {
                                    minDist = dist;
                                }
                            }
                        }
                    }
    
                    unsigned int value = static_cast<unsigned int>((minDist / cellSize) * 255);
                    data[i] = static_cast<unsigned char>(std::min(value, 255u));
                    i++;
                }
            }
        }
        
        return data;
    }
private:
    NoiseGeneration() = delete;
};

#endif
