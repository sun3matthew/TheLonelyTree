#include <engine/input.h>
#include <cassert>

glm::vec2 Input::lastMousePosition = glm::vec2(0.0f);
glm::vec2 Input::currentMousePosition = glm::vec2(0.0f);
bool Input::firstMouse = true;

bool Input::previousState[(int)KeyCode::MAX_KEYS] = {false};
bool Input::currentState[(int)KeyCode::MAX_KEYS] = {false};

bool Input::previousMouseState[(int)MouseButtonCode::MAX_BUTTONS] = {false};
bool Input::currentMouseState[(int)MouseButtonCode::MAX_BUTTONS] = {false};

std::queue<KeyCode> Input::charBuffer;  // Initialize buffer

GLFWwindow* Input::window = nullptr;

glm::vec2 Input::scroll = glm::vec2(0.0f);

void Input::setWindow(GLFWwindow* window, int width, int height){
    Input::window = window;

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetKeyCallback(window, keyCallback);
}

void Input::charCallback(GLFWwindow* window, unsigned int codepoint) {
    charBuffer.push((KeyCode)codepoint);
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_BACKSPACE:
            case GLFW_KEY_ENTER:
            case GLFW_KEY_TAB:
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_LEFT:
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_UP:
            case GLFW_KEY_DOWN:
                charBuffer.push((KeyCode)key);
                break;
            default:
                break;
        }
    }
}

void Input::clearCharBuffer() {
    while (!charBuffer.empty()) {
        charBuffer.pop();
    }
}

std::queue<KeyCode> Input::getCharBuffer() {
    return charBuffer;
}

void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
        lastMousePosition = glm::vec2(xpos, ypos);
        firstMouse = false;
    }

    currentMousePosition = glm::vec2(xpos, ypos);
}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    scroll = glm::vec2(xoffset, yoffset);
}

void Input::bindCursor(bool value){
    assert(window != nullptr);

    if (value){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

glm::vec2 Input::getMousePosition(){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

glm::vec2 Input::getMousePositionScreenSpace(int width, int height){
    glm::vec2 pos = getMousePosition();
    return glm::vec2( //! TODO IDFK why this is weird, see for windows
        (pos.x) * 2,
        (height / 2 - pos.y) * 2
    );
}

glm::vec2 Input::getMouseDelta(){
    return lastMousePosition - getMousePosition();
}

float Input::getMouseScroll(){
    return scroll.y;
}

float Input::getMouseScrollX(){
    return scroll.x;
}

bool Input::getMouse(MouseButtonCode button){
    return currentMouseState[(int)button];
}

bool Input::getMouseDown(MouseButtonCode button){
    return currentMouseState[(int)button] && !previousMouseState[(int)button];
}

bool Input::getMouseUp(MouseButtonCode button){
    return !currentMouseState[(int)button] && previousMouseState[(int)button];
}

void Input::processInput(){
    assert(window != nullptr);

    for (int i = 0; i < (int)KeyCode::MAX_KEYS; i++){
        previousState[i] = currentState[i];
        currentState[i] = glfwGetKey(window, i);
    }

    for (int i = 0; i < (int)MouseButtonCode::MAX_BUTTONS; i++){
        previousMouseState[i] = currentMouseState[i];
        currentMouseState[i] = glfwGetMouseButton(window, i);
    }

    lastMousePosition = currentMousePosition;
    scroll = glm::vec2(0.0f);
}

bool Input::getKey(KeyCode key){
    return currentState[(int)key];
}

bool Input::getKeyDown(KeyCode key){
    return currentState[(int)key] && !previousState[(int)key];
}

bool Input::getKeyUp(KeyCode key){
    return !currentState[(int)key] && previousState[(int)key];
}