#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <engine/shader.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>

class RenderObject{
    public:
        std::string shaderName;
        glm::mat4 modelMatrix;

        RenderObject(std::string name);
        virtual ~RenderObject() = default;

        void draw();
        virtual void drawCall(Shader*) = 0;
};

#endif