#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <engine/key_code.h>
#include <engine/mouse_button_code.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <queue>

class Input{
    public:
        static void setWindow(GLFWwindow* window, int width, int height);
        static void processInput();

        static void bindCursor(bool value);

        static bool getKey(KeyCode key);
        static bool getKeyDown(KeyCode key);
        static bool getKeyUp(KeyCode key);

        static glm::vec2 getMousePosition();
        static glm::vec2 getMouseDelta();

        static glm::vec2 getMousePositionScreenSpace(int width, int height);

        static float getMouseScroll();
        static float getMouseScrollX();

        static bool getMouse(MouseButtonCode button);
        static bool getMouseDown(MouseButtonCode button);
        static bool getMouseUp(MouseButtonCode button);

        static std::queue<KeyCode> getCharBuffer(); 
        static void clearCharBuffer(); 
    private:
        static void charCallback(GLFWwindow* window, unsigned int codepoint);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static std::queue<KeyCode> charBuffer;

        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static GLFWwindow* window;

        static bool previousState[(int)KeyCode::MAX_KEYS];
        static bool currentState[(int)KeyCode::MAX_KEYS];

        static bool previousMouseState[(int)MouseButtonCode::MAX_BUTTONS];
        static bool currentMouseState[(int)MouseButtonCode::MAX_BUTTONS];

        static glm::vec2 lastMousePosition;
        static glm::vec2 currentMousePosition;
        static bool firstMouse;

        static glm::vec2 scroll;


};  

#endif