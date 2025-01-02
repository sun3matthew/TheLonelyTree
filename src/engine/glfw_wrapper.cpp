#include "engine/glfw_wrapper.h"

#include <iostream>


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
}

GLFWWrapper::~GLFWWrapper(){
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

    return 0;
}

int GLFWWrapper::run(){
    start();
    if (!window){
        std::cout << "Window not created" << std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();
        glfwSwapBuffers(window);  // Swap the buffers (important for double buffering)
        glfwPollEvents();
    }
    return 0;
}
