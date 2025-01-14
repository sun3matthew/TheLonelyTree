#include <engine/glfw_wrapper.h>

#include <iostream>
#include <engine/input.h>

float GLFWWrapper::lastX = 0.0f;
float GLFWWrapper::lastY = 0.0f;

GLFWWrapper::GLFWWrapper(){
    if (!glfwInit()){
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL X.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for Mac for some reason..

    window = nullptr;

    fps = 0;
    fpsCounter = 0;
    fpsLastTime = 0.0;
}

GLFWWrapper::~GLFWWrapper(){
    for (Gameobject* gameobject : gameobjects)
        delete gameobject;

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);

    GLFWWrapper::lastX = width / 2.0f;
    GLFWWrapper::lastY = height / 2.0f;
}

int GLFWWrapper::createWindow(int width, int height, const char* title){
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, width * 2, height * 2);



    lastX = width / 2.0f;
    lastY = height / 2.0f;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Input::setWindow(window, width, height);

    glEnable(GL_DEPTH_TEST);  
    // glEnable(GL_CULL_FACE);  

    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

int GLFWWrapper::FPS(){
    return fps;
}

void GLFWWrapper::calculateFPS(){
    double currentTime = glfwGetTime();
    fpsCounter++;

    if (currentTime - fpsLastTime >= 1.0){
        fps = fpsCounter;
        fpsCounter = 0;
        fpsLastTime = currentTime;
    }
}

void GLFWWrapper::calculateDeltaTime(){
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
}

float GLFWWrapper::getDeltaTime(){
    return deltaTime;
}

Gameobject* GLFWWrapper::find(std::string name){
    for(Gameobject* gameobject : gameobjects)
        if(gameobject->name == name)
            return gameobject;
    return nullptr;
}

void GLFWWrapper::addGameobject(Gameobject* gameobject){
    gameobjects.push_back(gameobject);
}

void GLFWWrapper::removeGameobject(Gameobject* gameobject){
    gameobjects.remove(gameobject);
}



int GLFWWrapper::run(){
    start();
    if (!window){
        std::cout << "Window not created" << std::endl;
        return -1;
    }

    // TODO Investigate if you should essentially render the last frame and then while it is rendering to the double buffer, do the current frame processing, then swap the buffers.
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();
        for(Gameobject* gameobject : gameobjects)
            gameobject->update();

        for(Gameobject* gameobject : gameobjects)
            gameobject->lateUpdate();
        lateUpdate();

        // END OF FRAME
        RenderManager::instance.draw();

        glfwSwapBuffers(window);

        Input::processInput();
        glfwPollEvents();

        calculateFPS();
        calculateDeltaTime();
    }
    return 0;
}

void GLFWWrapper::lockCursor(bool lock){
    assert(window != nullptr);

    if(lock)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
