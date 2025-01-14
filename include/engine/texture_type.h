#ifndef TEXTURE_TYPE_H
#define TEXTURE_TYPE_H

#include <string>

enum class TextureType {
    Diffuse = 0,
    Specular,
    Glossy,
    CubeMap,
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
        case TextureType::CubeMap:
            return "cube_map";
        default:
            return "unknown";
    }
}

#endif // TEXTURE_TYPE_H