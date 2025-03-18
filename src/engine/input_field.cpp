
#include <engine/glfw_wrapper.h>

#include <engine/input_field.h>
#include <engine/input.h>

InputField::InputField(Font* font, glm::vec2 min, glm::vec2 max, float scale, glm::vec3 color)
    : RenderObjectComponent(new Text(font, "", min, max, scale, color))
{
    buffer = "";
    blinkCounter = 0.0f;
    cursorPosition = 0;
}

InputField::~InputField()
{
    // ! Holy F*cking Sh*t, c++ is so hard to be a bad programmer in
    // ! I need stricter rules to follow
    // if(textObject)
    //     delete textObject;
}


Text* InputField::getTextObject(){
    return dynamic_cast<Text*>(renderObject);
}

#include <iostream>
void InputField::update(){
    RenderObjectComponent::update();

    std::queue<KeyCode> charBuffer = Input::getCharBuffer();

    std::vector<KeyCode> specialKeys;

    int count = 0;
    while (!charBuffer.empty()) {
        KeyCode c = charBuffer.front();
        charBuffer.pop();

        std::string s = KeyCodeToString(c);
        if (s.size() == 1){
            buffer = buffer.substr(0, cursorPosition) + s + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
            cursorPosition++;
        }else{
            specialKeys.push_back(c);
        }
    }

    for(KeyCode c : specialKeys){
        if (c == KeyCode::KEY_TAB){
            // buffer += "";
            for (int i = 0; i < 4; i++){
                buffer = buffer.substr(0, cursorPosition) + " " + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
                cursorPosition++;
            }
        }

        if (c == KeyCode::KEY_ENTER){
            // buffer += "\n";
            buffer = buffer.substr(0, cursorPosition) + "\n" + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
            cursorPosition++;
        }

        if (c == KeyCode::KEY_BACKSPACE){
            buffer = buffer.substr(0, cursorPosition - 1) + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
            cursorPosition--;
        }

        if (c == KeyCode::KEY_LEFT){
            if (cursorPosition > 0)
                cursorPosition--;
        }

        if (c == KeyCode::KEY_RIGHT){
            if (cursorPosition < buffer.size())
                cursorPosition++;
        }
    }

    Text* textObject = getTextObject();

    // on mouse click, set cursor position to the closest character
    if (Input::getMouseDown(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        cursorPosition = buffer.size();

        glm::vec2 mousePosition = Input::getMousePositionScreenSpace(GLFWWrapper::width, GLFWWrapper::height);

        std::vector<AABB> characterAABBs = textObject->getCharacterAABBs();
        for (int i = 0; i < characterAABBs.size(); i++){
            if (characterAABBs[i].contains(mousePosition)){
                cursorPosition = i + 1;
                break;
            }
        }
    }

    textObject->setText(buffer);
    textObject->setCursorPosition(cursorPosition);

    textObject->setCursorVisible(blinkCounter < 0.5f);

    blinkCounter += GLFWWrapper::instance->getDeltaTime();
    if (blinkCounter > 1.0f){
        blinkCounter = 0.0f;
    }

}