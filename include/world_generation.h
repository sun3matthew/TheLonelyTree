#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

#include <vector>
#include <PerlinNoise.hpp>
#include <engine/mesh.h>

class WorldGeneration {
public:
    // Static methods
    static float getHeightAt(float x, float y);
    static Mesh* createWorld(unsigned int seedIn, float heightIn, float sizeIn, float densityIn);

    static float worldSize();
private:
    static unsigned int seed;
    static siv::PerlinNoise seededPerlin;
    static float height, size;

    // Private constructor to prevent instantiation
    WorldGeneration() = delete;
};

#endif // MESH_GENERATION_H
