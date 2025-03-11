#ifndef TEXTURE_TYPE_H
#define TEXTURE_TYPE_H

#include <string>

enum class TextureType {
    Diffuse = 0,
    Specular,
    Glossy,
    Normal,
    CubeMap,
    PerlinLane,
    DepthBuffer,
    FrameBuffer,
    Font,
    TDNoise,
    TDDetailNoise,
    MAX_TYPES
};

inline std::string TextureTypeToString(TextureType textureType){
    switch (textureType){
        case TextureType::Diffuse:
            return "diffuse";
        case TextureType::Specular:
            return "specular";
        case TextureType::Glossy:
            return "glossy";
        case TextureType::Normal:
            return "normal";
        case TextureType::CubeMap:
            return "cube_map";
        case TextureType::PerlinLane:
            return "perlin_lane";
        case TextureType::DepthBuffer:
            return "depth_buffer";
        case TextureType::FrameBuffer:
            return "frame_buffer";
        case TextureType::Font:
            return "font";
        case TextureType::TDNoise:
            return "td_noise";
        case TextureType::TDDetailNoise:
            return "td_detail_noise";
        default:
            return "unknown";
    }
}

#endif // TEXTURE_TYPE_H