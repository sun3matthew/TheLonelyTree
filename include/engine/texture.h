#ifndef TEXTURE_H
#define TEXTURE_H

#include <engine/texture_type.h>

class Texture
{
public:

    TextureType type;

    Texture(const char* path, TextureType type);
    Texture(const unsigned char* data, int width, int height, int nChannels, TextureType type);
    // TODO add a destructor to clean up openGL

    void bind(int textureUnit);
    int getID();
    int getTextureUnit();
    int getWidth();
    int getHeight();
    int getNrChannels();

    static Texture defaultDiffuse();
    static Texture defaultSpecular();
private:
    unsigned int ID;
    int textureUnit;
    int width, height, nChannels;

};

#endif