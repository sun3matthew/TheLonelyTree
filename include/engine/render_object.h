#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <engine/shader.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>

class RenderObject{
    public:
        std::vector<std::string> shaderNames;
        glm::mat4 modelMatrix;

        RenderObject();
        virtual ~RenderObject() = default;

        void addShader(std::string shaderName);

        void draw();
        virtual void drawCall(Shader*) = 0;
};

#endif