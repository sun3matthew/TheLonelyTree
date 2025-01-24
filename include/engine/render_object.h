#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <engine/shader.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>

class RenderObject{
    public:
        glm::mat4 modelMatrix;

        std::unordered_map<int, std::vector<std::string>> shaderNames;

        RenderObject();
        virtual ~RenderObject() = default;

        void addShader(std::string shaderName);
        void addShader(int i, std::string shaderName);

        void draw(int);
        virtual void drawCall(Shader*) = 0;
};

#endif