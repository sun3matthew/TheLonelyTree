#include <engine/shader.h>
#include <glad/glad.h> 
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

static char* readShaderFile(const char* filepath){
    // prepend ../ to the filepath
    char* newFilepath = (char*)malloc(strlen(filepath) + 4);
    strcpy(newFilepath, "../");
    strcat(newFilepath, filepath);

    FILE* file = fopen(newFilepath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}
Shader::Shader(std::string nameIn, const char* vertexPath, const char* fragmentPath, std::vector<std::string> canAcceptList)
    : Shader(nameIn, vertexPath, nullptr, fragmentPath, std::move(canAcceptList))
{}

Shader::Shader(std::string nameIn, const char* vertexPath, const char* geometryPath, const char* fragmentPath, std::vector<std::string> canAcceptList)
    : name(nameIn)
{
    for(std::string attribute : canAcceptList){
        canAccept.insert(attribute);
    }

    int  success;
    char infoLog[512];

    char *vertexShaderSource = readShaderFile(vertexPath);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    free(vertexShaderSource);

    unsigned int geometryShader;
    if(geometryPath){
        char *geometryShaderSource = readShaderFile(geometryPath);
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        free(geometryShaderSource);
    }

    char *fragmentShaderSource = readShaderFile(fragmentPath);
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    free(fragmentShaderSource);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    if (geometryPath)
        glAttachShader(ID, geometryShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(fragmentShader); 
    if (geometryPath)
        glDeleteShader(geometryShader);
    glDeleteShader(vertexShader);
}

std::string Shader::getName(){
    return name;
}

bool Shader::canAcceptAttribute(std::string attribute){
    return canAccept.count(attribute);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string &name, glm::vec3 value) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setMat4(const std::string &name, float *value) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setTexture(Texture* texture, int textureID) const{
    texture->bind(textureID);
    glUniform1i(glGetUniformLocation(ID, TextureTypeToString(texture->type).c_str()), textureID);
}

int Shader::getID(){
    return ID;
}