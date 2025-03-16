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
private:
    Texture* texture;
};

#endif // TEXT_H