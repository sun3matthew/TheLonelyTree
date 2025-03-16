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
    std::string getText();

    void setPosition(glm::vec2 min, glm::vec2 max) override;
    void setScale(float scale) override;

    void setCursorPosition(int position);
    void setCursorVisible(bool visible);

    std::vector<AABB>& getCharacterAABBs();
private:
    void recalculateCache();

    Font* font;

    std::string text;
    int cursorPosition;
    bool cursorVisible;

    std::vector<AABB> characterAABBs;
};

#endif // TEXT_H