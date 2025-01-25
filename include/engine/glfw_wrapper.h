#ifndef GLFW_WRAPPER_H
#define GLFW_WRAPPER_H


#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <engine/gameobject.h>
#include <engine/render_manager.h>
#include <engine/texture.h>

#include <list>
#include <string>

class GLFWWrapper{
    public:
        int run();

        GLFWWrapper();
        virtual ~GLFWWrapper();

        static float lastX, lastY;
        static float width, height; // ! Bad design
        static GLFWWrapper* instance;

        float getDeltaTime();
    protected:
        Gameobject* find(std::string);
        void addGameobject(Gameobject*);
        void removeGameobject(Gameobject*);

        virtual void start() = 0;
        virtual void update() = 0;
        virtual void lateUpdate() = 0;

        int createWindow(int width, int height, const char* title);
        int FPS();

        void lockCursor(bool lock);

    private:
        std::list<Gameobject*> gameobjects;
        
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