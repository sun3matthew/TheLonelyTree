#ifndef IMAGE_H
#define IMAGE_H

#include <engine/texture.h>
#include <engine/ui_render_object.h>


class Image : public UIRenderObject
{
public:
    Image(Texture* texture, glm::vec2 min, glm::vec2 max, glm::vec3 color);
    ~Image();

    void drawCall(Shader*) override;

    void updateTexture(Texture* texture);
    void setPosition(glm::vec2 min, glm::vec2 max);
    void setColor(glm::vec3 color);
private:
    Texture* texture;

    glm::vec2 min, max;
    glm::vec3 color;

    unsigned int VAO, VBO;
};

#endif // TEXT_H