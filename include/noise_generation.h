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
        int width = static_cast<int>(height * aspectRatio);
        std::vector<unsigned char> data(width * height);

        int i = 0;
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                data[i] = static_cast<unsigned char>(0xff * seededPerlin.octave2D_01(x * zoom, y * zoom, 1));
                i++;
            }
        }

        return new Texture(data.data(), width, height, 1, TextureType::PerlinLane);
    }

    static Texture GetCloudNoise(unsigned int seed, int resolution){
        std::vector<unsigned char> perlinData = TDPerlinNoise(seed, resolution, 0.04f, 8);
        std::vector<unsigned char> worleyData = TDWorleyNoise(seed, resolution, resolution / 10.0f, 3, 0.3f);

        std::vector<unsigned char> worleyData1 = TDWorleyNoise(seed + 491, resolution, resolution / 10.0f, 2, 0.3f);
        std::vector<unsigned char> worleyData2 = TDWorleyNoise(seed + 1033, resolution, resolution / 8.0f, 2, 0.3f);
        std::vector<unsigned char> worleyData3 = TDWorleyNoise(seed + 4212, resolution, resolution / 6.0f, 1, 0.3f);

        std::vector<unsigned char> data(resolution * resolution * resolution * 4);

        // copy perlin and worley noise to 3D texture
        int i = 0;
        int idx = 0;
        for(int z = 0; z < resolution; z++){
            for(int y = 0; y < resolution; y++){
                for(int x = 0; x < resolution; x++){
                    float worleyValue = (worleyData[idx]) / 255.0f;
                    float perlinValue = perlinData[idx] / 255.0f;
                    float perlinWorleyValue = 0.35f * worleyValue + 0.65f * perlinValue;
                    data[i] = static_cast<unsigned char>(0xff * perlinWorleyValue);

                    data[i + 1] = worleyData1[idx];
                    data[i + 2] = worleyData2[idx];
                    data[i + 3] = worleyData3[idx];

                    i += 4;
                    idx++;
                }
            }
        }

        return Texture(data.data(), resolution, resolution, resolution, 4, TextureType::TDNoise);
    }

    static Texture GetDetailedCloudNoise(unsigned int seed, int resolution){
        std::vector<unsigned char> worleyData1 = TDWorleyNoise(seed + 3391, resolution, resolution / 8.0f, 3, 0.3f);
        std::vector<unsigned char> worleyData2 = TDWorleyNoise(seed + 1033, resolution, resolution / 6.0f, 3, 0.3f);
        std::vector<unsigned char> worleyData3 = TDWorleyNoise(seed + 4212, resolution, resolution / 4.0f, 2, 0.3f);

        std::vector<unsigned char> data(resolution * resolution * resolution * 3);

        // copy worley noise to 3D texture
        int i = 0;
        int idx = 0;
        for(int z = 0; z < resolution; z++){
            for(int y = 0; y < resolution; y++){
                for(int x = 0; x < resolution; x++){
                    data[i + 0] = worleyData1[idx];
                    data[i + 1] = worleyData2[idx];
                    data[i + 2] = worleyData3[idx];

                    i += 3;
                    idx++;
                }
            }
        }

        return Texture(data.data(), resolution, resolution, resolution, 3, TextureType::TDDetailNoise);
    }

    static std::vector<unsigned char> TDPerlinNoise(unsigned int seed, int resolution, float zoom, int octaves){
        siv::PerlinNoise seededPerlin{seed};

        int height = resolution;
        int width = resolution;
        int depth = resolution;
        std::vector<unsigned char> data(width * height * depth);

        int i = 0;
        for (int z = 0; z < depth; z++){
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    data[i] = static_cast<unsigned char>(0xff * seededPerlin.octave3D_01(x * zoom, y * zoom, z * zoom, octaves));
                    i++;
                }
            }
        }

        return data;
    }
    
    static std::vector<unsigned char> TDWorleyNoise(unsigned int seed, int resolution, float initialFrequency, int octaves, float persistence){
        int width = resolution;
        int height = resolution;
        int depth = resolution;
        std::vector<unsigned char> data(width * height * depth);

        std::mt19937 rng(seed);
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        int cellSize = resolution / initialFrequency;
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
                    data[i] = 0xff - static_cast<unsigned char>(std::min(value, 255u));
                    i++;
                }
            }
        }

        // resample the generated noise at a higher frequency
        for(int i = 1; i < octaves; i++){
            int newFrequency = std::pow(2.0f, i);
            float newPersistance = std::pow(persistence, i);
            for(int z = 0; z < depth; z++){
                for(int y = 0; y < height; y++){
                    for(int x = 0; x < width; x++){
                        int xCoord = (x * newFrequency) % resolution;
                        int yCoord = (y * newFrequency) % resolution;
                        int zCoord = (z * newFrequency) % resolution;

                        int idx = x + y * resolution + z * resolution * resolution;
                        float value = data[xCoord + yCoord * resolution + zCoord * resolution * resolution] / 255.0f;
                        data[idx] = static_cast<unsigned char>(0xff * (value * newPersistance + (1.0f - newPersistance) * data[idx] / 255.0f));
                    }
                }
            }
        }

        return data;
    }

private:
    NoiseGeneration() = delete;
};

#endif
