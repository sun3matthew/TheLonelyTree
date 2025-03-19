#ifndef UI_RENDER_OBJECT_H
#define UI_RENDER_OBJECT_H

#include <engine/render_object.h>
#include <engine/aabb.h>

class UICanvas;

class UIRenderObject : public RenderObject
{
public:
    UIRenderObject();
    ~UIRenderObject();

    void drawCall(Shader*) override;

    virtual void setColor(glm::vec3 color);
    virtual void setPosition(glm::vec2 min, glm::vec2 max);
    virtual void setScale(float scale);

    AABB getAABB();
protected:
    unsigned int VAO, VBO;

    glm::vec3 color;
    float scale;
    AABB aabb;
};

#endif // UI_RENDER_OBJECT_H