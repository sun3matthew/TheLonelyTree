#ifndef UI_RENDER_OBJECT_H
#define UI_RENDER_OBJECT_H

#include <engine/render_object.h>

class UIRenderObject : public RenderObject
{
public:
    UIRenderObject();
    ~UIRenderObject();

    void drawCall(Shader*) override;
private:
    unsigned int VAO, VBO;
};

#endif // UI_RENDER_OBJECT_H