#ifndef GLFW_WRAPPER_H
#define GLFW_WRAPPER_H


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
        int FPS();
        int getDeltaTime();
    private:
        GLFWwindow* window;

        double fpsLastTime;
        int fpsCounter;
        int fps;
        void calculateFPS();

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        void calculateDeltaTime();
};  

#endif