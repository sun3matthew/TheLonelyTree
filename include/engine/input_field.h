#pragma once

#include <engine/component.h>
#include <engine/text.h>

#include <engine/ui_component.h>

class InputField : public UIComponent{
    public:
        InputField(Text* textObject);
        InputField(Text* textObject, std::string emptyString);
        ~InputField() override;

        void focused() override;
        void onUnfocused() override;

        Text* getTextObject();
    private:
        std::string buffer;
        std::string emptyString;

        float blinkCounter;
        int cursorPosition;
};