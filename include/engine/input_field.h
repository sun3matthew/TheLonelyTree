#pragma once

#include <engine/component.h>
#include <engine/text.h>

#include <engine/render_object_component.h>

class InputField : public RenderObjectComponent{
    public:
        InputField(Font* font, glm::vec2 min, glm::vec2 max, float scale, glm::vec3 color);
        ~InputField() override;

        void update() override;

        Text* getTextObject();
    private:
        std::string buffer;

        float blinkCounter;
        int cursorPosition;
};