#pragma once

#include <engine/component.h>
#include <engine/text.h>

#include <engine/ui_component.h>

class InputField : public UIComponent{
    public:
        InputField(Text* textObject);
        ~InputField() override;

        void focused() override;
        void onUnfocused() override;

        Text* getTextObject();
    private:
        std::string buffer;

        float blinkCounter;
        int cursorPosition;
};