
#include <engine/input_field.h>
#include <engine/input.h>

InputField::InputField(Text* textObject, std::string emptyString)
    : UIComponent(textObject), emptyString(emptyString)
{
    clickable = true;

    buffer = "";
    blinkCounter = 0.0f;
    cursorPosition = 0;

    onUnfocused();
}

InputField::InputField(Text* textObject)
    : InputField(textObject, "")
{}

InputField::~InputField()
{
    // ! Holy F*cking Sh*t, c++ is so easy to be a bad programmer in
    // ! I need stricter rules to follow
    // if(textObject)
    //     delete textObject;
}


Text* InputField::getTextObject(){
    return dynamic_cast<Text*>(renderObject);
}

void InputField::onUnfocused(){
    this->getTextObject()->setCursorVisible(false);
    if (buffer == ""){
        this->getTextObject()->setText(emptyString);
    }
}

void InputField::focused(){
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
            blinkCounter = 0.0f;
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
                blinkCounter = 0.0f;
            }
        }

        if (c == KeyCode::KEY_ENTER){
            // buffer += "\n";
            buffer = buffer.substr(0, cursorPosition) + "\n" + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
            cursorPosition++;
            blinkCounter = 0.0f;
        }

        if (c == KeyCode::KEY_BACKSPACE){
            if (cursorPosition > 0){
                buffer = buffer.substr(0, cursorPosition - 1) + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
                cursorPosition--;
                blinkCounter = 0.0f;
            }
        }

        if (c == KeyCode::KEY_LEFT){
            if (cursorPosition > 0){
                cursorPosition--;
                blinkCounter = 0.0f;
            }
        }

        if (c == KeyCode::KEY_RIGHT){
            if (cursorPosition < buffer.size()){
                cursorPosition++;
                blinkCounter = 0.0f;
            }
        }
    }

    Text* textObject = getTextObject();

    // on mouse click, set cursor position to the closest character
    if (Input::getMouseDown(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        cursorPosition = buffer.size();

        glm::vec2 mousePosition = Input::getMousePositionPixelSpace();

        std::vector<AABB> characterAABBs = textObject->getCharacterAABBs();
        for (int i = 0; i < characterAABBs.size(); i++){
            if (characterAABBs[i].contains(mousePosition)){
                cursorPosition = i + 1;
                blinkCounter = 0.0f;
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