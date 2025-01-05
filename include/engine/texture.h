#ifndef TEXTURE_H
#define TEXTURE_H

#include <engine/texture_type.h>

class Texture
{
public:
    static Texture defaultDiffuse;
    static Texture defaultSpecular;

    TextureType type;

    Texture(const char* path, TextureType type);
    Texture(const unsigned char* data, int width, int height, int nChannels, TextureType type);
    void bind(int textureUnit);
    int getID();
    int getTextureUnit();
    int getWidth();
    int getHeight();
    int getNrChannels();
private:
    unsigned int ID;
    int textureUnit;
    int width, height, nChannels;

};

#endif