#include "the_lonely_tree.h"

#include <iostream>
#include <glm/glm.hpp>

#include <engine/input.h>
#include <engine/mesh_generation.h>
#include <engine/texture_type.h>
#include <engine/frame_buffer_generation.h>

#include <engine/gltf_loader.h>
#include <engine/render_object_component.h>
#include <engine/render_object.h>

#include <engine/mesh.h>
#include <engine/grass.h>
#include <engine/shader.h>
#include <engine/light_directional.h>
#include <engine/light_spot.h>

#include <world_generation.h>

#include <iostream>

// Camera 
TheLonelyTree::TheLonelyTree()
{
    TheLonelyTree::instance = this;

    createWindow(800 * 1.5f, 600 * 1.5f, "The Lonely Tree");
    lockCursor(true);

    // TODO turn these into enums
    RenderManager::instance.addShader(
        new Shader("screen", "resources/shaders/screen.vert", "resources/shaders/screen.frag",
            std::vector<std::string>{"meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("shadowMap", "resources/shaders/shadowMap.vert", "resources/shaders/shadowMap.frag",
            std::vector<std::string>{"dirLightCamera", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("model", "resources/shaders/model.vert", "resources/shaders/model.frag",
            std::vector<std::string>{"camera", "dirLightCamera", "dirLight", "pointLights", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("skyBox", "resources/shaders/skybox.vert", "resources/shaders/skybox.frag",
            std::vector<std::string>{"cameraSkyBox", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("grass", "resources/shaders/grass.vert", "resources/shaders/grass.geom", "resources/shaders/grass.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));

    RenderManager::instance.addFrameBuffer(FrameBufferGeneration::BaseFrameBuffer(FRAME_BUFFER, GLFWWrapper::width, GLFWWrapper::height));
    RenderManager::instance.addFrameBuffer(FrameBufferGeneration::ShadowMap(SHADOW_BUFFER, 1024 * 6, 1024 * 6));

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TheLonelyTree::~TheLonelyTree(){
}

void TheLonelyTree::start(){
    numLights = 0;


    float worldSize = 1024 * 5;

    Gameobject* screenQuad = new Gameobject("Screen Quad");
    Mesh* screenQuadMesh = MeshGeneration::ScreenQuad();
    screenQuadMesh->textures.push_back(RenderManager::instance.getFrameBuffer(FRAME_BUFFER).textures[0]);
    screenQuadMesh->addShader(BASE_FAME_BUFFER, "screen");
    screenQuad->addComponent(new RenderObjectComponent(screenQuadMesh));
    addGameobject(screenQuad);

    Gameobject* camera = new Gameobject("Camera");
    this->camera = new Camera(
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        0.0f
        );
    camera->addComponent(this->camera);
    addGameobject(camera);
    camera->setPosition(glm::vec3(worldSize/2, 400, worldSize/2));

    Gameobject* directionalLight = new Gameobject("Directional Light");
    LightDirectional *light = new LightDirectional(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.30f, 0.30f, 0.30f),
        glm::vec3(0.40f, 0.40f, 0.40f),
        glm::vec3(0.9f, 0.9f, 0.9f));
    light->setWorldSize(worldSize);
    directionalLight->addComponent(light);
    addGameobject(directionalLight);

    Gameobject* tree = GLTFLoader::loadMesh("resources/models/tree/scene.gltf");
    std::list<Gameobject*> allChildren = tree->getAllChildren();
    for(Gameobject* child : allChildren){
        RenderObjectComponent* meshComp = child -> getComponent<RenderObjectComponent>();
        if(meshComp){
            Mesh* mesh = static_cast<Mesh*>(meshComp->getRenderObject());
            mesh->addShader(FRAME_BUFFER, "model");
            mesh->addShader(SHADOW_BUFFER, "shadowMap");
        }
    }
    addGameobject(tree);
    tree->setScale(glm::vec3(0.6f));
    tree->setPosition(glm::vec3(worldSize / 2, 300, worldSize / 2));

    Gameobject* world = new Gameobject("World");
    Mesh* terrainMesh = WorldGeneration::createWorld(12923952u, 60, worldSize, 4 * 2);
    terrainMesh->updateTexture(Texture::diffuse(0x50, 0x4D, 0x53));
    terrainMesh->addShader(FRAME_BUFFER, "model");
    terrainMesh->addShader(SHADOW_BUFFER, "shadowMap");
    world->addComponent(new RenderObjectComponent(terrainMesh));
    addGameobject(world);

    // Gameobject* water = new Gameobject("Water");
    // Mesh* waterMesh = MeshGeneration::Plane(64, 64);
    // waterMesh->addShader("model");
    // water->addComponent(new RenderObjectComponent(waterMesh));
    // addGameobject(water);
    // water->setScale(glm::vec3(worldSize * 8, 1, worldSize * 8));

    // float worldSize = 500.0f;
    // plane->setPosition(glm::vec3(worldSize/2, -50, worldSize/2));
    

    // Gameobject* sphere = new Gameobject("Sphere");
    // Mesh* sphereMesh = MeshGeneration::Sphere(32, 32);
    // sphereMesh->addShader("model");
    // sphereMesh->addShader(1, "shadowMap");
    // sphere->addComponent(new RenderObjectComponent(sphereMesh));
    // addGameobject(sphere);
    // sphere->setPosition(glm::vec3(worldSize/2 + 100, 450, worldSize/2));
    // sphere->setScale(glm::vec3(90.0f));

    // ! Cube bugged
    Gameobject* cube = new Gameobject("Cube");
    Mesh* cubeMesh = MeshGeneration::Cube();
    cubeMesh->addShader(FRAME_BUFFER, "model");
    cubeMesh->addShader(SHADOW_BUFFER, "shadowMap");
    cube->addComponent(new RenderObjectComponent(cubeMesh));
    addGameobject(cube);
    cube->setPosition(glm::vec3(worldSize/2 - 200, 360, worldSize/2));
    cube->setScale(glm::vec3(90.0f));

    Gameobject* grass = new Gameobject("Grass");
    Grass* grassMesh = new Grass();
    grassMesh->addShader(FRAME_BUFFER, "grass");
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
    skyBoxMesh->addShader(FRAME_BUFFER, "skyBox");
    skyBoxMesh->updateTexture(Texture(faces));
    skyBox->addComponent(new RenderObjectComponent(skyBoxMesh));
    addGameobject(skyBox);
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
            camera->getGameobject()->getPosition(),
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