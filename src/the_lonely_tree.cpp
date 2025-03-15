#include "the_lonely_tree.h"

#include <iostream>
#include <glm/glm.hpp>

#include <engine/input.h>
#include <engine/mesh_generation.h>
#include <engine/texture_type.h>
#include <engine/frame_buffer_generation.h>
#include <engine/crypto.h>
#include <engine/save_file.h>

#include <engine/gltf_loader.h>
#include <engine/render_object_component.h>
#include <engine/render_object.h>

#include <engine/mesh.h>
#include <engine/grass.h>
#include <engine/shader.h>
#include <engine/light_directional.h>
#include <engine/light_spot.h>

#include <engine/font.h>
#include <engine/text.h>
#include <engine/image.h>

#include <world_generation.h>
#include <noise_generation.h>
#include <leaf_manager.h>
#include <tree_renderer_component.h>

#include <iostream>

#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>

#include <steam/steam_api.h>

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

    // createWindow(800 * 1.5f, 600 * 1.5f, "The Lonely Tree");
    createWindow(900 * 1.5f, 650 * 1.5f, "The Lonely Tree");
    lockCursor(true);

    // TODO turn these into enums
    RenderManager::instance.addShader(
        new Shader("screen", "../resources/shaders/screen.vert", "../resources/shaders/screen.frag",
            std::vector<std::string>{"meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("text", "../resources/shaders/ui.vert", "../resources/shaders/text.frag",
            std::vector<std::string>{}));
    RenderManager::instance.addShader(
        new Shader("image", "../resources/shaders/ui.vert", "../resources/shaders/image.frag",
            std::vector<std::string>{}));

    RenderManager::instance.addShader(
        new Shader("shadowMap", "../resources/shaders/shadowMap.vert", "../resources/shaders/shadowMap.frag",
            std::vector<std::string>{"dirLightCamera", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("treeShadowMap", "../resources/shaders/tree.vert", "../resources/shaders/tree_sm.geom", "../resources/shaders/shadowMap.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));
    RenderManager::instance.addShader(
        new Shader("leafShadowMap", "../resources/shaders/leaf.vert", "../resources/shaders/leaf_sm.geom", "../resources/shaders/shadowMap.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));
    
    RenderManager::instance.addShader(
        new Shader("model", "../resources/shaders/model.vert", "../resources/shaders/model.frag",
            std::vector<std::string>{"camera", "dirLightCamera", "dirLight", "pointLights", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("skyBox", "../resources/shaders/skybox.vert", "../resources/shaders/skybox.frag",
            std::vector<std::string>{"cameraSkyBox", "meshTextures", "dayTime"}));
    // RenderManager::instance.addShader(
    //     new Shader("skyBox", "../resources/shaders/skybox.vert", "../resources/shaders/clouds.frag",
    //         std::vector<std::string>{"cameraSkyBox", "meshTextures", "dayTime"}));

    RenderManager::instance.addShader(
        new Shader("grass", "../resources/shaders/grass.vert", "../resources/shaders/grass.geom", "../resources/shaders/grass.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));
    RenderManager::instance.addShader(
        new Shader("shadowMap", "../resources/shaders/shadowMap.vert", "../resources/shaders/shadowMap.frag",
            std::vector<std::string>{"dirLightCamera", "model", "meshTextures"}));
    RenderManager::instance.addShader(
        new Shader("tree", "../resources/shaders/tree.vert", "../resources/shaders/tree.geom", "../resources/shaders/tree.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));
    RenderManager::instance.addShader(
        new Shader("leaf", "../resources/shaders/leaf.vert", "../resources/shaders/leaf.geom", "../resources/shaders/leaf.frag",
            std::vector<std::string>{"camera", "dirLight", "dirLightCamera"}));

    RenderManager::instance.addFrameBuffer(FrameBufferGeneration::BaseFrameBuffer(FRAME_BUFFER, GLFWWrapper::width, GLFWWrapper::height));
    RenderManager::instance.addFrameBuffer(FrameBufferGeneration::ShadowMap(SHADOW_BUFFER, 1024 * 6, 1024 * 6));

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    font = new Font("../resources/fonts/arial.ttf", 48);

    SaveFile::Initialize("SunCats", "TheLonelyTree");

    if (SteamAPI_Init()) {
        // std::cout << "Hello, Steam! User: " << SteamFriends()->GetPersonaName() << std::endl;
        steamUsername = SteamFriends()->GetPersonaName();
        steamID = SteamUser()->GetSteamID().ConvertToUint64();

        SaveFile::Save("steamID", std::to_string(steamID));
    }else{
        steamUsername = "Offline";
        steamID = 0;

        std::string steamIDString = SaveFile::Load("steamID");
        if (steamIDString != ""){
            steamID = std::stoull(steamIDString);
        }

        std::cerr << "Failed to initialize Steam API!" << std::endl;
    }
}

TheLonelyTree::~TheLonelyTree(){
    // delete camera; //! Do not delete, just a reference to a component
    delete font;
    if (treeManager)
        delete treeManager;

    SteamAPI_Shutdown();
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

    Gameobject* cameraGameobject = new Gameobject("Camera");
    this->camera = new Camera(
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        0.0f
        );
    cameraGameobject->addComponent(this->camera);
    addGameobject(cameraGameobject);
    cameraGameobject->setPosition(glm::vec3(worldSize/2, 400, worldSize/2));
    camera->originPosition = cameraGameobject->getPosition();

    Gameobject* directionalLight = new Gameobject("Directional Light");
    // glm::vec3 color = glm::vec3(246.0f / 255.0f, 222.0f / 255.0f, 200.0f / 255.0f);
    glm::vec3 color = glm::vec3(233.0f / 255.0f, 172.0f / 255.0f, 99.0f / 255.0f);
    // glm::vec3 color = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
    LightDirectional *light = new LightDirectional(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::normalize(glm::vec3(0.25f, 0.25f, 0.25f) * color),
        glm::normalize(glm::vec3(0.40f, 0.40f, 0.40f) * color),
        glm::normalize(glm::vec3(0.9f, 0.9f, 0.9f) * color));

    light->setWorldSize(worldSize);
    directionalLight->addComponent(light);
    addGameobject(directionalLight);

    Gameobject* tree = GLTFLoader::loadMesh("../resources/models/tree/scene.gltf");
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
    tree->setScale(glm::vec3(0.7f));
    tree->setPosition(glm::vec3(worldSize / 2, 300, worldSize / 2));

    Gameobject* bird = GLTFLoader::loadMesh("../resources/models/butterfly/scene.gltf");
    allChildren = bird->getAllChildren();
    for(Gameobject* child : allChildren){
        RenderObjectComponent* meshComp = child -> getComponent<RenderObjectComponent>();
        if(meshComp){
            Mesh* mesh = static_cast<Mesh*>(meshComp->getRenderObject());
            mesh->addShader(FRAME_BUFFER, "model");
            mesh->addShader(SHADOW_BUFFER, "shadowMap");
        }
    }
    addGameobject(bird);
    bird->setScale(glm::vec3(800.0f));
    bird->setPosition(glm::vec3(worldSize / 2 - 500, 508, worldSize / 2 + 500));

    int seed = 12923952u;

    Gameobject* world2 = new Gameobject("World");
    Mesh* terrainMesh2 = WorldGeneration::createWorld(seed, 60, worldSize, 4, 10);
    terrainMesh2->updateTexture(Texture::diffuse(0x50, 0x4D, 0x53));
    // terrainMesh2->addShader(FRAME_BUFFER, "model");
    terrainMesh2->addShader(SHADOW_BUFFER, "shadowMap");
    world2->addComponent(new RenderObjectComponent(terrainMesh2));
    addGameobject(world2);

    Gameobject* world = new Gameobject("World");
    Mesh* terrainMesh = WorldGeneration::createWorld(seed, 60, worldSize, 4 * 2, 0);
    terrainMesh->updateTexture(Texture::diffuse(0x50, 0x4D, 0x53));
    terrainMesh->addShader(FRAME_BUFFER, "model");
    terrainMesh->addShader(SHADOW_BUFFER, "shadowMap");
    world->addComponent(new RenderObjectComponent(terrainMesh));
    addGameobject(world);

    unsigned int numLeafTypes = 2;

    Entry entry = Entry(
        "2021-09-01",
        "The Lonely Tree",
        "The Lonely Tree is a project that aims to create a procedurally generated tree that can be used in a variety of applications. The tree is generated using a combination of L-systems and Perlin noise to create a realistic looking tree. The tree is then rendered using OpenGL and GLSL to create a realistic 3D model. The tree can be viewed from any angle and can be used in a variety of applications such as games, simulations, and visualizations."
    );

    Texture branchDiffuse = Texture("../resources/textures/tree/Diffuse.jpeg", TextureType::Diffuse);
    Texture branchNormal = Texture("../resources/textures/tree/Normal.png", TextureType::Normal);

    // //! This is the worst code I have ever written
    userTree = new UserTree("https://7sqvdwegyf.execute-api.us-west-2.amazonaws.com", "/default/the-lonely-tree");
    std::string baseBranchID = userTree->read(userTree->constructKey(EntryType::UserIDBranchID, Crypto::toHex(steamID)));
    // if (baseBranchID == ""){
    //     treeManager = new TreeManager();
    // }else{
    //     treeManager = new TreeManager(Crypto::hexToLong(baseBranchID));
    // }
    treeManager = new TreeManager(5842433818981);
    userTree->updateTreeManager(steamID, treeManager);

    treeManager->rootBranch()->pushBackTexture(branchDiffuse);
    treeManager->rootBranch()->pushBackTexture(branchNormal);
    treeManager->rootBranch()->addShader(FRAME_BUFFER, "tree");
    treeManager->rootBranch()->addShader(SHADOW_BUFFER, "treeShadowMap");
    treeManager->rootBranch()->getLeafManager()->addShader(FRAME_BUFFER, "leaf");
    treeManager->rootBranch()->getLeafManager()->addShader(SHADOW_BUFFER, "leafShadowMap");
    int numNodes = 50;
    for (int i = 0; i < numNodes; i++){
        treeManager->rootBranch()->addNode(entry);
    }
    treeManager->rootBranch()->recalculateVertices();

    // build tree
    // srand(time(0));

    int treeSeed = rand();
    // int seed = rand();
    std::cout << "Seed: " << treeSeed << std::endl;
    srand(treeSeed);

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
            int numBranches = rand() % 4 + 1;
            // numBranches = 4;
            for(int i = 0; i < numBranches; i++){
                int numNodes = rand() % (10 * depth) + 4;
                // std::cout << "Num Nodes: " << numNodes << std::endl;
                counter++;

                // random node from 0 to numNodes
                int totalNodes = branch->getNumNodes();
                int randomNode = rand() % (int)(totalNodes); // TODO make better
                TreeBranch* newBranch = treeManager->addBranch(branch->getID(), randomNode);
                newBranch->pushBackTexture(branchDiffuse);
                newBranch->pushBackTexture(branchNormal);
                newBranch->addShader(FRAME_BUFFER, "tree");
                newBranch->addShader(SHADOW_BUFFER, "treeShadowMap");
                newBranch->getLeafManager()->addShader(FRAME_BUFFER, "leaf");
                newBranch->getLeafManager()->addShader(SHADOW_BUFFER, "leafShadowMap");
                for (int i = 0; i < numNodes; i++)
                    newBranch->addNode(entry);
                newBranch->recalculateVertices();

                // for( int i = 0; i < newBranch->getNumNodes(); ){
                //     TreeVertex* vertex = newBranch->getNode(i)->getVertexData();
                //     LeafKey key = newBranch->getLeafManager()->getNewLeafKey();
                //     newBranch->getLeafManager()->writeLeafData(key, vertex->position, vertex->direction);
                //     // i += rand() % 3 + 1;
                //     i++;
                // }
                // newBranch->getLeafManager()->writeDataToGPU();

                if (depth - 1 > 0){
                    // std::cout << "PUSH " << (depth - 1) << std::endl;
                    branchStack.push({newBranch, depth - 1});
                }
            }
        }
    }


    Gameobject* treeManager = new Gameobject("Tree Manager");
    treeManager->addComponent(new TreeRendererComponent(this->treeManager));
    treeManager->addComponent(this->userTree);
    treeManager->setPosition(glm::vec3(worldSize/2, 280, worldSize/2));
    treeManager->setScale(glm::vec3(100.0f));
    // addGameobject(treeManager);

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
    // cube->setPosition(glm::vec3(worldSize/2 - 200, 0, worldSize/2));
    // cube->setScale(glm::vec3(10.0f));

    grass = new Gameobject("Grass");
    Grass* grassMesh = new Grass();
    grassMesh->addShader(FRAME_BUFFER, "grass");
    grass->addComponent(new RenderObjectComponent(grassMesh));
    addGameobject(grass);

    // std::string path = "../resources/textures/cubemaps/cloudDay/";
    std::string path = "../resources/textures/cubemaps/cloudSunset/";
    std::vector<std::string> faces = {
        path + "right.jpg",
        path + "left.jpg",
        path + "top.jpg",
        path + "bottom.jpg",
        path + "front.jpg",
        path + "back.jpg"
    };
    Texture tdNoise = NoiseGeneration::GetCloudNoise(seed, 128);
    Texture tdDetailedNoise = NoiseGeneration::GetDetailedCloudNoise(seed, 128);
    Gameobject* skyBox = new Gameobject("SkyBox");
    Mesh* skyBoxMesh = MeshGeneration::SkyMap();
    skyBoxMesh->addShader(FRAME_BUFFER, "skyBox");
    skyBoxMesh->updateTexture(Texture(faces));
    skyBoxMesh->updateTexture(tdNoise);
    skyBoxMesh->updateTexture(tdDetailedNoise);
    skyBox->addComponent(new RenderObjectComponent(skyBoxMesh));
    addGameobject(skyBox);


    ui = new Gameobject("UI");
    ui->active = false;
    addGameobject(ui);

    Texture* texture = new Texture("../resources/textures/UI/Button.png", TextureType::Diffuse);
    Gameobject* image = new Gameobject("Image");
    Image* imageMesh = new Image(texture, glm::vec2(0.1, 0.1), glm::vec2(0.9, 0.9), glm::vec3(1.0f, 1.0f, 1.0f));
    imageMesh->addShader(FRAME_BUFFER, "image");
    image->addComponent(new RenderObjectComponent(imageMesh));
    image->setParent(ui);

    Gameobject* text = new Gameobject("Text");
    textMesh = new Text(font, "The Lonely Tree", glm::vec2(0.2, 0.75), glm::vec2(0.8, 0.25), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    textMesh->addShader(FRAME_BUFFER, "text");
    text->addComponent(new RenderObjectComponent(textMesh));
    text->setParent(ui);

    std::cout << "Steam Username: " << steamUsername << std::endl;
    Gameobject* text2 = new Gameobject("Text");
    Text* textMesh2 = new Text(font, steamUsername, glm::vec2(0.0, 0.0), glm::vec2(0.8, 0.25), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    textMesh2->addShader(FRAME_BUFFER, "text");
    text2->addComponent(new RenderObjectComponent(textMesh2));
    text2->setParent(ui);
}

void TheLonelyTree::update(){
    if (Input::getKeyDown(KeyCode::KEY_F12)){
        screenShotMode = !screenShotMode;
        if (screenShotMode){
            lockCursor(false);
        }else{
            lockCursor(true);
        }
    }
    if(screenShotMode){
        return;
    }

    if(!ui->active){
        grass->active = true;

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
    }else{
        // grass->active = false;
        
        for (int i = 0; i < (int)KeyCode::MAX_KEYS; i++){
            if (Input::getKeyDown((KeyCode)i)){
                std::string key = KeyCodeToString((KeyCode)i);
                if (!Input::getKey(KeyCode::KEY_LEFT_SHIFT)){
                    if(key[0] >= 'A' && key[0] <= 'Z')
                        key[0] = std::tolower(key[0]);
                }
                entry += key;
                textMesh->setText(entry);
            }
        }

        if (Input::getKeyDown(KeyCode::KEY_BACKSPACE)){
            entry = entry.substr(0, entry.size() - 1);
        }
        if (Input::getKeyDown(KeyCode::KEY_ENTER)){
            userTree->addEntry(Entry(
                getCurrentDateTime(),
                steamUsername,
                entry
            ));
        }

    }

    if (Input::getKeyUp(KeyCode::KEY_ESCAPE)){
        ui->active = !ui->active;
    }
    // std::cout << "FPS: " << FPS() << std::endl;
}

void TheLonelyTree::lateUpdate() {}