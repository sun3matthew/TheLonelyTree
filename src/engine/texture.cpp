#include <engine/texture.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

Texture Texture::defaultDiffuse(){
    return Texture((const unsigned char[]) {0xFF, 0xFF, 0xFF}, 1, 1, 3, TextureType::Diffuse);
}
Texture Texture::defaultSpecular(){
    return Texture((const unsigned char[]) {0xFF, 0xFF, 0xFF}, 1, 1, 3, TextureType::Specular);
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
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture " << path << std::endl;
    }
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
} 


void Texture::bind(int textureUnit){
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