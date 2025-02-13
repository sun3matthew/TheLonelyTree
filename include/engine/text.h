#ifndef TEXT_H
#define TEXT_H

#include <engine/font.h>
#include <string>
#include <glm/glm.hpp>

#include <engine/ui_render_object.h>

class Text : public UIRenderObject
{
public:
    Text(Font* font, std::string text, glm::vec2 min, glm::vec2 max, float scale, glm::vec3 color);
    ~Text();

    void drawCall(Shader*) override;

    void updateFont(Font* font);
    void setText(std::string text);
    void setPosition(glm::vec2 min, glm::vec2 max);
    void setColor(glm::vec3 color);
    void setScale(float scale);
private:
    Font* font;

    std::string text;

    glm::vec2 min, max;
    glm::vec3 color;
    float scale;

    unsigned int VAO, VBO;
};

#endif // TEXT_H