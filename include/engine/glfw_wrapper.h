#ifndef GLAD_WRAPPER_H
#define GLAD_WRAPPER_H


#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class GLFWWrapper{
    public:
        int run();

        GLFWWrapper();
        virtual ~GLFWWrapper();

        static float lastX, lastY;
    protected:
        virtual void start() = 0;
        virtual void update() = 0;

        int createWindow(int width, int height, const char* title);
// framebuffer_size_callback
    private:
        GLFWwindow* window;



};  

#endif