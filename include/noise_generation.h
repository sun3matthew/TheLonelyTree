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

    static Texture GetCloudNoise(unsigned int seed, int resolution){
        unsigned char* perlinData = TDPerlinNoise(seed, resolution, 0.06f, 8);
        unsigned char* worleyData = TDWorleyNoise(seed, resolution, resolution / 10.0f, 4, 0.3f);

        unsigned char* worleyData1 = TDWorleyNoise(seed + 491, resolution, resolution / 16.0f, 4, 0.3f);
        unsigned char* worleyData2 = TDWorleyNoise(seed + 1033, resolution, resolution / 8.0f, 3, 0.3f);
        unsigned char* worleyData3 = TDWorleyNoise(seed + 4212, resolution, resolution / 4.0f, 2, 0.3f);

        unsigned char data[resolution * resolution * resolution * 4];

        // copy perlin noise to 3D texture
        int i = 0;
        int idx = 0;
        for(int z = 0; z < resolution; z++){
            for(int y = 0; y < resolution; y++){
                for(int x = 0; x < resolution; x++){
                    float worleyValue = (0xff - worleyData[idx]) / 255.0f;
                    float perlinValue = perlinData[idx] / 255.0f;
                    float perlinWorleyValue = 0.5f * worleyValue + 0.5f * perlinValue;
                    data[i] = static_cast<unsigned char>(0xff * perlinWorleyValue);

                    data[i + 1] = 0xff - worleyData1[idx];
                    data[i + 2] = 0xff - worleyData2[idx];
                    data[i + 3] = 0xff - worleyData3[idx];

                    i += 4;
                    idx++;
                }
            }
        }




        delete[] perlinData;
        delete[] worleyData;
        delete[] worleyData1;
        delete[] worleyData2;
        delete[] worleyData3;

        return Texture(data, resolution, resolution, resolution, 4, TextureType::TDNoise);
    }

    static unsigned char* TDPerlinNoise(unsigned int seed, int resolution, float zoom, int octaves){
        siv::PerlinNoise seededPerlin{seed};

        int height = resolution;
        int width = resolution;
        int depth = resolution;
        unsigned char* data = new unsigned char[width * height * depth];

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
    
    static unsigned char* TDWorleyNoise(unsigned int seed, int resolution, float initialFrequency, int octaves, float persistence){
        int width = resolution;
        int height = resolution;
        int depth = resolution;
        unsigned char* data = new unsigned char[width * height * depth];
    
        std::mt19937 rng(seed);
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
        // Precompute feature points for each octave
        std::vector<std::vector<std::vector<std::vector<glm::vec3>>>> allFeaturePoints(octaves);
        for (int octave = 0; octave < octaves; ++octave) {
            float frequency = initialFrequency * std::pow(2.0f, octave);
            float cellSize = resolution / frequency;
            int gridSize = static_cast<int>(std::ceil(resolution / cellSize));
    
            allFeaturePoints[octave].resize(gridSize,
                std::vector<std::vector<glm::vec3>>(gridSize,
                    std::vector<glm::vec3>(gridSize)));
    
            for (int gx = 0; gx < gridSize; gx++) {
                for (int gy = 0; gy < gridSize; gy++) {
                    for (int gz = 0; gz < gridSize; gz++) {
                        allFeaturePoints[octave][gx][gy][gz] = glm::vec3(
                            (gx + dist(rng)) * cellSize,
                            (gy + dist(rng)) * cellSize,
                            (gz + dist(rng)) * cellSize
                        );
                    }
                }
            }
        }
    
        // Compute Worley noise with fBm
        int i = 0;
        for (int z = 0; z < depth; z++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float totalNoise = 0.0f;
                    float amplitude = 1.0f;
                    float frequency = initialFrequency;
                    glm::vec3 voxelPos(x, y, z);
    
                    for (int octave = 0; octave < octaves; ++octave) {
                        float cellSize = resolution / frequency;
                        int gridSize = static_cast<int>(std::ceil(resolution / cellSize));
    
                        int gx = static_cast<int>(x / cellSize);
                        int gy = static_cast<int>(y / cellSize);
                        int gz = static_cast<int>(z / cellSize);
    
                        float minDist = std::numeric_limits<float>::max();
    
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                for (int dz = -1; dz <= 1; dz++) {
                                    int neighborX = (gx + dx + gridSize) % gridSize;
                                    int neighborY = (gy + dy + gridSize) % gridSize;
                                    int neighborZ = (gz + dz + gridSize) % gridSize;
    
                                    glm::vec3 point = allFeaturePoints[octave][neighborX][neighborY][neighborZ];
    
                                    glm::vec3 wrappedDist = voxelPos - point;
                                    wrappedDist.x -= resolution * std::round(wrappedDist.x / resolution);
                                    wrappedDist.y -= resolution * std::round(wrappedDist.y / resolution);
                                    wrappedDist.z -= resolution * std::round(wrappedDist.z / resolution);
    
                                    float dist = glm::dot(wrappedDist, wrappedDist);
                                    if (dist < minDist) {
                                        minDist = dist;
                                    }
                                }
                            }
                        }
    
                        minDist = std::sqrt(minDist);
                        totalNoise += (minDist / cellSize) * amplitude;
    
                        frequency *= 2.0f;
                        amplitude *= persistence;
                    }
    
                    // Normalize and store the result
                    unsigned int value = static_cast<unsigned int>((totalNoise / (2.0f - 1.0f)) * 255);
                    data[i++] = static_cast<unsigned char>(std::min(value, 255u));
                }
            }
        }
    
        return data;
    }
private:
    NoiseGeneration() = delete;
};

#endif
