#ifndef NOISE_GENERATION_H
#define NOISE_GENERATION_H

#include <vector>
#include <PerlinNoise.hpp>
#include <engine/texture.h>

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

private:
    NoiseGeneration() = delete;
};

#endif
