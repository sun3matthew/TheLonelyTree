#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <vector>

#define PI 3.14159265359
#define TAU 6.28318530718


static float lastX = 800, lastY = 600;
static bool firstMouse = true;


int main(void){
    glfwInit();
    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL X.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for Mac for some reason..

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    


    glViewport(0, 0, 1600, 1200); // for some reason its double the size of the window



    while(!glfwWindowShouldClose(window)){
        // Rendering commands here
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
