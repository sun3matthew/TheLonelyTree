#include "the_lonely_tree.h"

#include <iostream>
#include <glm/glm.hpp>

#include <engine/input.h>
#include <engine/mesh_generation.h>
#include <engine/texture_type.h>

#include <engine/gltf_loader.h>
#include <engine/mesh_comp.h>

#include <engine/mesh.h>
#include <engine/shader.h>
#include <engine/light_directional.h>
#include <engine/light_spot.h>

// Camera 
TheLonelyTree::TheLonelyTree()
{
    createWindow(800, 600, "The Lonely Tree");
    lockCursor(true);
}

TheLonelyTree::~TheLonelyTree(){
}

void TheLonelyTree::start(){
    numLights = 0;

    RenderManager::instance.addShader(
        new Shader("model", "resources/shaders/model.vert", "resources/shaders/model.frag",
            std::vector<std::string>{"camera", "dirLight", "pointLights"}));

    Gameobject* camera = new Gameobject("Camera");
    this->camera = new Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        0.0f
        );
    camera->addComponent(this->camera);
    addGameobject(camera);

    Gameobject* directionalLight = new Gameobject("Directional Light");
    directionalLight->addComponent(new LightDirectional(
        glm::vec3(-3.0f, -1.0f, -0.0f),
        glm::vec3(0.35f, 0.35f, 0.35f),
        glm::vec3(0.65f, 0.65f, 0.65f),
        glm::vec3(0.9f, 0.9f, 0.9f)));
    addGameobject(directionalLight);

    // Gameobject* backpack = GLTFLoader::loadMesh("resources/models/backpack/scene.gltf");
    // Gameobject* backpack = GLTFLoader::loadMesh("resources/models/backpack/scene.gltf");
    // addGameobject(backpack);
    // backpack->setScale(glm::vec3(0.01f));

    Gameobject* sphere = new Gameobject("Sphere");
    sphere->addComponent(new MeshComp(MeshGeneration::Sphere(32, 32)));
    addGameobject(sphere);
    sphere->setPosition(glm::vec3(0, 3, 0));

    Gameobject* plane = new Gameobject("Plane");
    plane->addComponent(new MeshComp(MeshGeneration::Terrain(12923952u, 64, 64)));
    addGameobject(plane);

    plane->setPosition(glm::vec3(0, -3, 0));
    plane->setScale(glm::vec3(90, 10, 90));
}

void TheLonelyTree::update(){
    if (Input::getKey(KeyCode::KEY_W))
        camera->ProcessKeyboard(FORWARD, getDeltaTime());
    if (Input::getKey(KeyCode::KEY_S))
        camera->ProcessKeyboard(BACKWARD, getDeltaTime());
    if (Input::getKey(KeyCode::KEY_A))
        camera->ProcessKeyboard(LEFT, getDeltaTime());
    if (Input::getKey(KeyCode::KEY_D))
        camera->ProcessKeyboard(RIGHT, getDeltaTime());

    camera->ProcessMouseScroll(Input::getMouseScroll());

    glm::vec2 mouseDelta = Input::getMouseDelta();
    camera->ProcessMouseMovement(-mouseDelta.x, mouseDelta.y);

    if (Input::getKeyUp(KeyCode::KEY_SPACE)){
        Gameobject* gameobject = new Gameobject();
        gameobject->addComponent(new LightSpot(
            camera->Position,
            glm::vec3(1.0f, 0.09f, 0.032f),
            numLights,
            glm::vec3(0.15f, 0.15f, 0.15f),
            glm::vec3(0.6f, 0.6f, 0.6f),
            glm::vec3(0.8f, 0.8f, 0.8f)));
        addGameobject(gameobject);

        numLights++;
        RenderManager::instance.getShadersAccepting("pointLights")[0]->setInt("numPointLights", numLights);
    }
    std::cout << "FPS: " << FPS() << std::endl;
}

void TheLonelyTree::lateUpdate() {}