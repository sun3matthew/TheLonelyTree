#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <engine/texture.h>
#include <unordered_set>

class Shader
{
private:
    unsigned int ID;
    std::string name;
    std::unordered_set<std::string> canAccept;

public:
    //TODO add getters
  
    // constructor reads and builds the shader
    Shader(std::string nameIn, const char* vertexPath, const char* fragmentPath, std::vector<std::string> canAcceptList);
    Shader(std::string nameIn, const char* vertexPath, const char* geometryPath, const char* fragmentPath, std::vector<std::string> canAcceptList);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, float *value) const;
    void setTexture(Texture* texture, int textureID) const;

    int getID();
    std::string getName();
    bool canAcceptAttribute(std::string);
};

#endif