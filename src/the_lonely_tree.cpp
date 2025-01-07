#include "the_lonely_tree.h"

#include <iostream>
#include <glm/glm.hpp>

#include <engine/input.h>
#include <engine/mesh_generation.h>
#include <engine/texture_type.h>

#include <engine/gltf_loader.h>

// Camera 
TheLonelyTree::TheLonelyTree()
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)), 
    directionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.35f, 0.35f, 0.35f),
        glm::vec3(0.95f, 0.95f, 0.95f),
        glm::vec3(0.9f, 0.9f, 0.9f))
{
    createWindow(800, 600, "The Lonely Tree");
    lockCursor(true);

}

TheLonelyTree::~TheLonelyTree(){
    delete meshShader;
}

void TheLonelyTree::start(){
    meshShader = new Shader("resources/shaders/model.vert", "resources/shaders/model.frag");
    // meshes.push_back(MeshGeneration::Sphere(64, 64));

    Texture diffuse("resources/models/backpack/textures/Scene_-_Root_baseColor.jpeg", TextureType::Diffuse);
    Texture specular("resources/models/backpack/textures/Scene_-_Root_specular.jpg", TextureType::Specular);
    std::vector<Texture> textures{diffuse, specular};
    std::vector<Mesh> backpack = GLTFLoader::loadMesh("resources/models/backpack/scene.gltf");
    for (Mesh &mesh : backpack){
        mesh.updateTextures(textures);
        meshes.push_back(mesh);
    }

    
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
    modelMat = glm::scale(modelMat, glm::vec3(0.05, 0.05, 0.05));
    meshShader->setMat4("model", glm::value_ptr(modelMat));

    meshShader->setInt("numPointLights", pointLights.size());
    for(LightSpot light : pointLights)
        light.writeToShader(*meshShader);

    for (int i = 0; i < meshes.size(); i++){
        meshes[i].draw(*meshShader);
    }


    if (Input::getMouseDown(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button Down" << std::endl;
    }

    if (Input::getMouse(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button" << std::endl;
    }


    if (Input::getMouseUp(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button Up" << std::endl;
    }

    if (Input::getKeyUp(KeyCode::KEY_SPACE)){
        pointLights.push_back(LightSpot(
            camera.Position,
            glm::vec3(1.0f, 0.09f, 0.032f),
            pointLights.size(),
            glm::vec3(0.15f, 0.15f, 0.15f),
            glm::vec3(0.6f, 0.6f, 0.6f),
            glm::vec3(0.8f, 0.8f, 0.8f)));
    }

    // std::cout << "FPS: " << FPS() << std::endl;



}