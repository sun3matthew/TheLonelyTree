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
#include <engine/http_client.h>

#include <world_generation.h>
#include <tree_manager.h>
#include <tree_renderer_component.h>

#include <iostream>

#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>

std::string getCurrentDateTime() {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert to local time
    std::tm localTime = *std::localtime(&currentTime);

    // Create a string stream
    std::ostringstream dateTimeStream;
    dateTimeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return dateTimeStream.str(); // Return the string representation
}

// Camera 
TheLonelyTree::TheLonelyTree()
{
    TheLonelyTree::instance = this;

    createWindow(800 * 1.5f, 600 * 1.5f, "The Lonely Tree");
    lockCursor(true);

    // prepend ../ to the filepath
    // char* newFilepath = (char*)malloc(strlen(filepath) + 4);
    // strcpy(newFilepath, "../");
    // strcat(newFilepath, filepath);


    // TODO turn these into enums
    RenderManager::instance.addShader(
        new Shader("screen", "../resources/shaders/screen.vert", "../resources/shaders/screen.frag",
            std::vector<std::string>{"meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("shadowMap", "../resources/shaders/shadowMap.vert", "../resources/shaders/shadowMap.frag",
            std::vector<std::string>{"dirLightCamera", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("model", "../resources/shaders/model.vert", "../resources/shaders/model.frag",
            std::vector<std::string>{"camera", "dirLightCamera", "dirLight", "pointLights", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("skyBox", "../resources/shaders/skybox.vert", "../resources/shaders/skybox.frag",
            std::vector<std::string>{"cameraSkyBox", "meshTextures", "dayTime"}));
    RenderManager::instance.addShader(
        new Shader("grass", "../resources/shaders/grass.vert", "../resources/shaders/grass.geom", "../resources/shaders/grass.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));
    RenderManager::instance.addShader(
        new Shader("shadowMap", "../resources/shaders/shadowMap.vert", "../resources/shaders/shadowMap.frag",
            std::vector<std::string>{"dirLightCamera", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("tree", "../resources/shaders/tree.vert", "../resources/shaders/tree.geom", "../resources/shaders/tree.frag",
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
    // delete camera; //! Do not delete, just a reference to a component
    delete treeManager;
}

void TheLonelyTree::start(){
    // std::string url = "https://7sqvdwegyf.execute-api.us-west-2.amazonaws.com";
    // std::string dataPath = "/default/the-lonely-tree";
    // HttpClient client(url, dataPath);

    // std::string key = "exampleKey";
    // std::string value = getCurrentDateTime();

    // if(client.write(key, value)){
    //     std::cout << "Write successful" << std::endl;
    // }else{
    //     std::cout << "Write failed" << std::endl;
    // }

    // std::string readValue = client.read(key);
    // std::cout << "Read value: " << readValue << std::endl;

    numLights = 0;


    float worldSize = 1024 * 5;

    Gameobject* screenQuad = new Gameobject("Screen Quad");
    Mesh* screenQuadMesh = MeshGeneration::ScreenQuad();
    screenQuadMesh->textures.push_back(RenderManager::instance.getFrameBuffer(FRAME_BUFFER).textures[0]);
    screenQuadMesh->addShader(BASE_FAME_BUFFER, "screen");
    screenQuad->addComponent(new RenderObjectComponent(screenQuadMesh));
    addGameobject(screenQuad);

    Gameobject* cameraGameobject = new Gameobject("Camera");
    this->camera = new Camera(
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        0.0f
        );
    cameraGameobject->addComponent(this->camera);
    addGameobject(cameraGameobject);
    cameraGameobject->setPosition(glm::vec3(worldSize/2, 400, worldSize/2));

    Gameobject* directionalLight = new Gameobject("Directional Light");
    LightDirectional *light = new LightDirectional(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.40f, 0.40f, 0.40f),
        glm::vec3(0.9f, 0.9f, 0.9f));
    light->setWorldSize(worldSize);
    directionalLight->addComponent(light);
    addGameobject(directionalLight);

    // Gameobject* tree = GLTFLoader::loadMesh("../resources/models/tree/scene.gltf");
    // std::list<Gameobject*> allChildren = tree->getAllChildren();
    // for(Gameobject* child : allChildren){
    //     RenderObjectComponent* meshComp = child -> getComponent<RenderObjectComponent>();
    //     if(meshComp){
    //         Mesh* mesh = static_cast<Mesh*>(meshComp->getRenderObject());
    //         mesh->addShader(FRAME_BUFFER, "model");
    //         mesh->addShader(SHADOW_BUFFER, "shadowMap");
    //     }
    // }
    // addGameobject(tree);
    // tree->setScale(glm::vec3(0.6f));
    // tree->setPosition(glm::vec3(worldSize / 2, 300, worldSize / 2));

    int seed = 12923952u;

    // Gameobject* world2 = new Gameobject("World");
    // Mesh* terrainMesh2 = WorldGeneration::createWorld(seed, 60, worldSize, 4, 10);
    // terrainMesh2->updateTexture(Texture::diffuse(0x50, 0x4D, 0x53));
    // terrainMesh2->addShader(SHADOW_BUFFER, "shadowMap");
    // world2->addComponent(new RenderObjectComponent(terrainMesh2));
    // addGameobject(world2);

    Gameobject* world = new Gameobject("World");
    Mesh* terrainMesh = WorldGeneration::createWorld(seed, 60, worldSize, 4 * 2, 0);
    terrainMesh->updateTexture(Texture::diffuse(0x50, 0x4D, 0x53));
    terrainMesh->addShader(FRAME_BUFFER, "model");
    terrainMesh->addShader(SHADOW_BUFFER, "shadowMap");
    world->addComponent(new RenderObjectComponent(terrainMesh));
    addGameobject(world);


    Entry entry = Entry();
    entry.date = "2021-09-01";
    entry.name = "The Lonely Tree";
    entry.data = "The Lonely Tree is a project that aims to create a procedurally generated tree that can be used in a variety of applications. The tree is generated using a combination of L-systems and Perlin noise to create a realistic looking tree. The tree is then rendered using OpenGL and GLSL to create a realistic 3D model. The tree can be viewed from any angle and can be used in a variety of applications such as games, simulations, and visualizations.";

    Texture branchDiffuse = Texture("../resources/textures/tree/Diffuse.jpeg", TextureType::Diffuse);
    Texture branchNormal = Texture("../resources/textures/tree/Normal.png", TextureType::Normal);


    treeManager = new TreeManager();
    treeManager->rootBranch()->addShader(FRAME_BUFFER, "tree");
    treeManager->rootBranch()->addShader(SHADOW_BUFFER, "shadowMap");
    treeManager->rootBranch()->pushBackTexture(branchDiffuse);
    treeManager->rootBranch()->pushBackTexture(branchNormal);
    int numNodes = 60;
    for (int i = 0; i < numNodes; i++){
        treeManager->rootBranch()->addNode(entry);
    }
    treeManager->rootBranch()->recalculateVertices();

    std::stack <std::pair<TreeBranch*, int>> branchStack;
    branchStack.push({treeManager->rootBranch(), 3});
    int counter = 0;
    while(!branchStack.empty()){
        std::pair<TreeBranch*, int> branchPair = branchStack.top();
        branchStack.pop();
        TreeBranch* branch = branchPair.first;
        int depth = branchPair.second;
        // std::cout << "POP " << depth << std::endl;

        if (depth > 0){
            int numBranches = rand() % 2 + 3;
            numBranches = 4;
            for(int i = 0; i < numBranches; i++){
                int numNodes = rand() % (10 * depth) + 4;
                std::cout << "Num Nodes: " << numNodes << std::endl;
                counter++;
                // random node from 0 to numNodes
                int totalNodes = branch->getNumNodes();
                int randomNode = rand() % (totalNodes); // TODO make better
                TreeBranch* newBranch = treeManager->addBranch(branch->getID(), randomNode);
                newBranch->pushBackTexture(branchDiffuse);
                newBranch->pushBackTexture(branchNormal);
                newBranch->addShader(FRAME_BUFFER, "tree");
                newBranch->addShader(SHADOW_BUFFER, "shadowMap");
                for (int i = 0; i < numNodes; i++)
                    newBranch->addNode(entry);
                newBranch->recalculateVertices();
                if (depth - 1 > 0){
                    // std::cout << "PUSH " << (depth - 1) << std::endl;
                    branchStack.push({newBranch, depth - 1});
                }
            }
        }
    }


    // TreeBranch* secondBranch =  treeManager->addBranch(treeManager->rootBranch()->getID(), 80);
    // secondBranch->addShader(FRAME_BUFFER, "tree");
    // secondBranch->addShader(SHADOW_BUFFER, "shadowMap");
    // glm::vec3 direction = glm::normalize(glm::vec3(0, 1, 1));
    // for (int i = 0; i < numNodes; i++){
    //     glm::vec3 randomOffset = glm::vec3(
    //         (rand() % 1000) / 1000.0f - 0.5f,
    //         0,
    //         (rand() % 1000) / 1000.0f - 0.5f
    //     );
    //     float dist = 0.10;
    //     secondBranch->addNode(direction + randomOffset, dist, entry);
    // }

    // treeManager->rootBranch()->addNode(glm::vec3(0, 1, 1), 200, entry);

    Gameobject* treeManager = new Gameobject("Tree Manager");
    treeManager->addComponent(new TreeRendererComponent(this->treeManager));
    addGameobject(treeManager);
    treeManager->setPosition(glm::vec3(worldSize/2, 280, worldSize/2));
    treeManager->setScale(glm::vec3(100.0f));

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
    // sphereMesh->addShader(FRAME_BUFFER, "model");
    // sphereMesh->addShader(SHADOW_BUFFER, "shadowMap");
    // sphere->addComponent(new RenderObjectComponent(sphereMesh));
    // addGameobject(sphere);
    // sphere->setPosition(glm::vec3(worldSize/2 + 100, 450, worldSize/2));
    // sphere->setScale(glm::vec3(90.0f));

    // ! Cube bugged
    // Gameobject* cube = new Gameobject("Cube");
    // Mesh* cubeMesh = MeshGeneration::Cube();
    // cubeMesh->addShader(FRAME_BUFFER, "model");
    // cubeMesh->addShader(SHADOW_BUFFER, "shadowMap");
    // cube->addComponent(new RenderObjectComponent(cubeMesh));
    // addGameobject(cube);
    // cube->setPosition(glm::vec3(worldSize/2 - 200, 360, worldSize/2));
    // cube->setScale(glm::vec3(90.0f));

    Gameobject* grass = new Gameobject("Grass");
    Grass* grassMesh = new Grass();
    grassMesh->addShader(FRAME_BUFFER, "grass");
    grass->addComponent(new RenderObjectComponent(grassMesh));
    addGameobject(grass);

    std::string path = "../resources/textures/cubemaps/sky/";
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
    // std::cout << "FPS: " << FPS() << std::endl;
}

void TheLonelyTree::lateUpdate() {}