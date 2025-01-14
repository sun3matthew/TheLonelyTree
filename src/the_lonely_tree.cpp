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
    createWindow(800 * 1.5f, 600 * 1.5f, "The Lonely Tree");
    lockCursor(true);
}

TheLonelyTree::~TheLonelyTree(){
}

void TheLonelyTree::start(){
    numLights = 0;

    RenderManager::instance.addShader(
        new Shader("model", "resources/shaders/model.vert", "resources/shaders/model.frag",
            std::vector<std::string>{"camera", "dirLight", "pointLights"}));
    RenderManager::instance.addShader(
        new Shader("skyBox", "resources/shaders/skybox.vert", "resources/shaders/skybox.frag",
            std::vector<std::string>{"cameraSkyBox"}));

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
    // std::list<Gameobject*> allChildren = backpack->getAllChildren();
    // for(Gameobject* child : allChildren){
    //     MeshComp* meshComp = child -> getComponent<MeshComp>();
    //     if(meshComp){
    //         meshComp->getMesh()->addShader("model");
    //     }
    // }
    // addGameobject(backpack);
    // backpack->setScale(glm::vec3(0.01f));

    Gameobject* plane = new Gameobject("Plane");
    Mesh* terrainMesh = MeshGeneration::Terrain(12923952u, 256, 256);
    terrainMesh->addShader("model");
    plane->addComponent(new MeshComp(terrainMesh));
    addGameobject(plane);

    Gameobject* sphere = new Gameobject("Sphere");
    Mesh* sphereMesh = MeshGeneration::Sphere(32, 32);
    sphereMesh->addShader("model");
    sphere->addComponent(new MeshComp(sphereMesh));
    addGameobject(sphere);

    Gameobject* cube = new Gameobject("Cube");
    Mesh* cubeMesh = MeshGeneration::Cube();
    cubeMesh->addShader("model");
    cube->addComponent(new MeshComp(cubeMesh));
    addGameobject(cube);

    std::string path = "resources/textures/cubemaps/sky/";
    std::vector<std::string> faces = {
        path + "right.jpg",
        path + "left.jpg",
        path + "top.jpg",
        path + "bottom.jpg",
        path + "front.jpg",
        path + "back.jpg"
    };
    Gameobject* skyBox = new Gameobject("SkyBox");
    Mesh* skyBoxMesh = MeshGeneration::SkyMap();
    skyBoxMesh->addShader("skyBox");
    skyBoxMesh->updateTexture(Texture(faces));
    skyBox->addComponent(new MeshComp(skyBoxMesh));
    addGameobject(skyBox);

    sphere->setPosition(glm::vec3(0, 3, 0));
    cube->setPosition(glm::vec3(0, -3, 0));

    float worldSize = 1200.0f;
    plane->setPosition(glm::vec3(0, -50, 0));
    plane->setScale(glm::vec3(worldSize, 50, worldSize));
    

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
    // std::cout << "FPS: " << FPS() << std::endl;
}

void TheLonelyTree::lateUpdate() {}