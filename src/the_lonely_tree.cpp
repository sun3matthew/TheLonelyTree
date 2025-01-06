#include "the_lonely_tree.h"

#include <iostream>
#include <glm/glm.hpp>

#include <engine/input.h>
#include <engine/mesh_generation.h>

// Camera 
TheLonelyTree::TheLonelyTree()
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)), 
    directionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.15f, 0.15f, 0.15f),
        glm::vec3(0.6f, 0.6f, 0.6f),
        glm::vec3(0.7f, 0.7f, 0.7f))
{
    createWindow(800, 600, "The Lonely Tree");
    lockCursor(true);

}

TheLonelyTree::~TheLonelyTree(){
    delete meshShader;
}

void TheLonelyTree::start(){
    meshShader = new Shader("resources/shaders/model.vert", "resources/shaders/model.frag");
    meshes.push_back(MeshGeneration::Sphere(64, 64));
}

void TheLonelyTree::update(){
    if (Input::getKey(KeyCode::KEY_W))
        camera.ProcessKeyboard(FORWARD, getDeltaTime());
    if (Input::getKey(KeyCode::KEY_S))
        camera.ProcessKeyboard(BACKWARD, getDeltaTime());
    if (Input::getKey(KeyCode::KEY_A))
        camera.ProcessKeyboard(LEFT, getDeltaTime());
    if (Input::getKey(KeyCode::KEY_D))
        camera.ProcessKeyboard(RIGHT, getDeltaTime());

    camera.ProcessMouseScroll(Input::getMouseScroll());

    glm::vec2 mouseDelta = Input::getMouseDelta();
    camera.ProcessMouseMovement(-mouseDelta.x, mouseDelta.y);

    meshShader->use();
    camera.writeToShader(*meshShader);
    directionalLight.writeToShader(*meshShader);
    glm::mat4 modelMat(1);
    meshShader->setMat4("model", glm::value_ptr(modelMat));

    meshes[0].draw(*meshShader);

    if (Input::getMouseDown(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button Down" << std::endl;
    }

    if (Input::getMouse(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button" << std::endl;
    }


    if (Input::getMouseUp(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button Up" << std::endl;
    }

    if (Input::getKey(KeyCode::KEY_SPACE)){
        std::cout << "Mouse Position: " << Input::getMousePosition().x << ", " << Input::getMousePosition().y << std::endl;
        std::cout << "Mouse Delta: " << Input::getMouseDelta().x << ", " << Input::getMouseDelta().y << std::endl;

        std::cout << "Mouse Scroll: " << Input::getMouseScroll() << std::endl;
        std::cout << "Mouse Scroll X: " << Input::getMouseScrollX() << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

    // std::cout << "FPS: " << FPS() << std::endl;



}