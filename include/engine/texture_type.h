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
    ShadowMap,
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
        case TextureType::ShadowMap:
            return "shadow_map";
        default:
            return "unknown";
    }
}

#endif // TEXTURE_TYPE_H