#include "the_lonely_tree.h"

#include <iostream>
#include <glm/glm.hpp>

#include <engine/input.h>
#include <engine/mesh_generation.h>
#include <engine/texture_type.h>

#include <engine/gltf_loader.h>
#include <engine/render_object_component.h>
#include <engine/render_object.h>

#include <engine/mesh.h>
#include <engine/grass.h>
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
    RenderManager::instance.addShader(
        new Shader("grass", "resources/shaders/grass.vert", "resources/shaders/grass.geom", "resources/shaders/grass.frag",
            std::vector<std::string>{"camera", "dirLight"}));

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
        glm::vec3(0.45f, 0.45f, 0.45f),
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.9f, 0.9f, 0.9f)));
    addGameobject(directionalLight);

    // Gameobject* backpack = GLTFLoader::loadMesh("resources/models/backpack/scene.gltf");
    // std::list<Gameobject*> allChildren = backpack->getAllChildren();
    // for(Gameobject* child : allChildren){
    //     RenderObjectComponent* meshComp = child -> getComponent<RenderObjectComponent>();
    //     if(meshComp){
    //         meshComp->getMesh()->addShader("model");
    //     }
    // }
    // addGameobject(backpack);
    // backpack->setScale(glm::vec3(0.01f));

    Gameobject* plane = new Gameobject("Plane");
    Mesh* terrainMesh = MeshGeneration::Terrain(12923952u, 256, 256);
    terrainMesh->addShader("model");
    plane->addComponent(new RenderObjectComponent(terrainMesh));
    addGameobject(plane);
    float worldSize = 500.0f;
    plane->setPosition(glm::vec3(worldSize/2, -50, worldSize/2));
    plane->setScale(glm::vec3(worldSize, 50, worldSize));
    

    // Gameobject* sphere = new Gameobject("Sphere");
    // Mesh* sphereMesh = MeshGeneration::Sphere(32, 32);
    // sphereMesh->addShader("model");
    // sphere->addComponent(new RenderObjectComponent(sphereMesh));
    // addGameobject(sphere);
    // sphere->setPosition(glm::vec3(0, 3, 0));

    Gameobject* cube = new Gameobject("Cube");
    Mesh* cubeMesh = MeshGeneration::Cube();
    cubeMesh->addShader("model");
    cube->addComponent(new RenderObjectComponent(cubeMesh));
    addGameobject(cube);

    Gameobject* grass = new Gameobject("Grass");
    Grass* grassMesh = new Grass();
    grassMesh->addShader("grass");
    grass->addComponent(new RenderObjectComponent(grassMesh));
    addGameobject(grass);

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
    skyBox->addComponent(new RenderObjectComponent(skyBoxMesh));
    addGameobject(skyBox);

    cube->setPosition(glm::vec3(0, -3, 0));
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