#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <engine/shader.h>
#include <string>

class RenderObject{
    public:
        std::string shaderName;

        RenderObject(std::string name);
        virtual ~RenderObject() = default;

        void draw();
        virtual void drawCall(Shader*) = 0;
};

#endif