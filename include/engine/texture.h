#ifndef TEXTURE_H
#define TEXTURE_H

#include <engine/texture_type.h>
#include <vector>

class Texture
{
public:

    TextureType type;

    Texture(const char* path, TextureType type);
    Texture(const unsigned char* data, int width, int height, int nChannels, TextureType type, bool generateMipMap = true);
    Texture(std::vector<std::string> faces);
    Texture(int w, int h, unsigned int channelType, unsigned int channelCompType, TextureType textureType);
    Texture(const unsigned char* data, int w, int h, int d, int nC, TextureType textureType);
    // TODO add a destructor to clean up openGL

    void bind(int textureUnit);
    int getID();
    int getTextureUnit();
    int getWidth();
    int getHeight();
    int getNrChannels();

    static Texture defaultDiffuse();
    static Texture defaultNormal();
    static Texture defaultSpecular();
    static Texture defaultGlossy();
    static Texture defaultShadow();
    static std::vector<Texture> defaultTextures();

    static Texture diffuse(unsigned char r, unsigned char g, unsigned char b);
    static Texture specular(unsigned char r, unsigned char g, unsigned char b);
    static Texture glossy(unsigned char r);
private:
    unsigned int ID;
    int textureUnit;
    int width, height, depth, nChannels;

};

#endif