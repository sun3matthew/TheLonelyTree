#include <engine/texture.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

Texture Texture::defaultDiffuse(){
    return Texture((const unsigned char[]) {0xFF, 0xFF, 0xFF}, 1, 1, 3, TextureType::Diffuse);
}
Texture Texture::defaultNormal(){
    return Texture((const unsigned char[]) {0x80, 0x80, 0xFF}, 1, 1, 3, TextureType::Normal);
}
Texture Texture::defaultSpecular(){
    return Texture((const unsigned char[]) {0xFF, 0xFF, 0xFF}, 1, 1, 3, TextureType::Specular);
}
Texture Texture::defaultGlossy(){
    return Texture((const unsigned char[]) {0xFF}, 1, 1, 1, TextureType::Glossy);
}
std::vector<Texture> Texture::defaultTextures(){
    return {
        Texture::defaultDiffuse(),
        Texture::defaultNormal(),
        Texture::defaultSpecular(),
        Texture::defaultGlossy()
    };
}

Texture Texture::diffuse(unsigned char r, unsigned char g, unsigned char b){
    return Texture((const unsigned char[]) {r, g, b}, 1, 1, 3, TextureType::Diffuse);
}
Texture Texture::specular(unsigned char r, unsigned char g, unsigned char b){
    return Texture((const unsigned char[]) {r, g, b}, 1, 1, 3, TextureType::Specular);
}
Texture Texture::glossy(unsigned char amt){
    return Texture((const unsigned char[]) {0xFF}, 1, 1, 1, TextureType::Glossy);
}

Texture::Texture(const char* path, TextureType textureType) : type(textureType){

    glGenTextures(1, &ID);  
    glBindTexture(GL_TEXTURE_2D, ID);

    stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.

    // target, mipmap level, internal format, width, height, 0, format, type, data
    unsigned char *data = stbi_load(path, &width, &height, &nChannels, 0);
    if(data){
        if (nChannels == 3){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }else if (nChannels == 4){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }else if (nChannels == 1){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture " << path << std::endl;
    }
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const unsigned char* data, int w, int h, int nC, TextureType textureType) 
    : width(w), height(h), nChannels(nC), type(textureType)
{
    glGenTextures(1, &ID);  
    glBindTexture(GL_TEXTURE_2D, ID);

    // target, mipmap level, internal format, width, height, 0, format, type, data
    if (nChannels == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }else if (nChannels == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else if (nChannels == 1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
} 

Texture::Texture(std::vector<std::string> faces)
{
    type = TextureType::CubeMap;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void Texture::bind(int textureUnit){
    // TODO seperate CubeMaps out of Texture
    if(type == TextureType::CubeMap){
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        return;
    }
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    this->textureUnit = textureUnit;
}

int Texture::getID(){
    return ID;
}

int Texture::getTextureUnit(){
    return textureUnit;
}

int Texture::getWidth(){
    return width;
}

int Texture::getHeight(){
    return height;
}

int Texture::getNrChannels(){
    return nChannels;
}