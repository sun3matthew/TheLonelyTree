#include "the_lonely_tree.h"

#include <glm/glm.hpp>

#include <engine/logger.h>
#include <engine/input.h>
#include <engine/mesh_generation.h>
#include <engine/texture_type.h>
#include <engine/frame_buffer_generation.h>
#include <engine/crypto.h>

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
TheLonelyTree::TheLonelyTree() : GLFWWrapper("TheLonelyTree", "SunCats")
{
    createWindow(800 * 1.5f, 500 * 1.5f, "The Lonely Tree");
    // createWindow(900 * 1.5f, 650 * 1.5f, "The Lonely Tree");

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
        Logger::log("ERROR: Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    font = new Font("../resources/fonts/arial.ttf", 48);

    Audio::instance.load("summer", "../resources/audio/summer.wav");
    Audio::instance.load("test", "../resources/audio/test.wav");

    if (SteamAPI_Init()) {
        steamUsername = SteamFriends()->GetPersonaName();
        steamID = SteamUser()->GetSteamID().ConvertToUint64();

        Logger::log("Steam ID: " + std::to_string(steamID));

        SaveFile::write("steamID", std::to_string(steamID));
    }else{
        steamUsername = "Offline";
        steamID = 0;

        std::string steamIDString = SaveFile::read("steamID");
        if (steamIDString != ""){
            steamID = std::stoull(steamIDString);
        }

        Logger::log("ERROR: Failed to initialize Steam API!");
    }
}


// !!!! You need to take a fine tooth comb and find all the memory leaks. 
// ! Technically there arn't any for this game since every thing loaded will never be unloaded
// ! But its the principle.
TheLonelyTree::~TheLonelyTree(){
    // delete camera; //! Do not delete, just a reference to a component
    delete font;
    // if (userTree) //! also do not delete, it gets attached a gameobject
    //     delete userTree;

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
    // tree->setScale(glm::vec3(0.7f));
    // tree->setPosition(glm::vec3(worldSize / 2, 300, worldSize / 2));

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
        0,
        "2021-09-01",
        "The Lonely Tree",
        "The Lonely Tree is a project that aims to create a procedurally generated tree that can be used in a variety of applications. The tree is generated using a combination of L-systems and Perlin noise to create a realistic looking tree. The tree is then rendered using OpenGL and GLSL to create a realistic 3D model. The tree can be viewed from any angle and can be used in a variety of applications such as games, simulations, and visualizations."
    );

    Texture branchDiffuse = Texture("../resources/textures/tree/Diffuse.jpeg", TextureType::Diffuse);
    Texture branchNormal = Texture("../resources/textures/tree/Normal.png", TextureType::Normal);

    // //! This is the worst code I have ever written
    if (steamID == 0){
        Logger::log("WARNING: Steam ID is 0");
    }
    userTree = new UserTree(
        "https://7sqvdwegyf.execute-api.us-west-2.amazonaws.com",
        "/default/the-lonely-tree",
        steamID,
        branchDiffuse,
        branchNormal
    ); 
    // bool success;
    // std::string baseBranchID = userTree->read(constructKey(EntryType::UserIDBranchID, Crypto::toHex(steamID)));
    // if (baseBranchID == ""){
    //     treeManager = new TreeManager(0);
    // }else{
    //     treeManager = new TreeManager(Crypto::hexToLong(baseBranchID));
    // }

    // // treeManager = new TreeManager(5842433818981);
    // userTree->updateTreeManager(steamID, treeManager);

    // treeManager->rootBranch()->pushBackTexture(branchDiffuse);
    // treeManager->rootBranch()->pushBackTexture(branchNormal);
    // treeManager->rootBranch()->addShader(FRAME_BUFFER, "tree");
    // treeManager->rootBranch()->addShader(SHADOW_BUFFER, "treeShadowMap");
    // treeManager->rootBranch()->getLeafManager()->addShader(FRAME_BUFFER, "leaf");
    // treeManager->rootBranch()->getLeafManager()->addShader(SHADOW_BUFFER, "leafShadowMap");
    // int numNodes = 50;
    // for (int i = 0; i < numNodes; i++){
    //     treeManager->rootBranch()->addNode(entry);
    // }
    // treeManager->rootBranch()->recalculateVertices();

    // build tree
    // srand(time(0));

    // int treeSeed = rand();
    // // int seed = rand();
    // Logger::log("Seed: " + std::to_string(treeSeed));
    // srand(treeSeed);

    // std::stack <std::pair<TreeBranch*, int>> branchStack;
    // for(int i = 0; i < 4; i++)
    //     branchStack.push({treeManager->rootBranch(), 3});
    // int counter = 0;
    // while(!branchStack.empty()){
    //     std::pair<TreeBranch*, int> branchPair = branchStack.top();
    //     branchStack.pop();
    //     TreeBranch* branch = branchPair.first;
    //     int depth = branchPair.second;

    //     if (depth > 0){
    //         int numBranches = rand() % 4 + 1;
    //         // numBranches = 4;
    //         for(int i = 0; i < numBranches; i++){
    //             float randPercent = (float)rand() / (float)RAND_MAX;
    //             randPercent = randPercent * 0.5 + 0.25;
    //             int numNodes = (int)(pow(randPercent * 100 * depth + 4, 0.5f));
    //             std::cout << "Num Nodes: " << numNodes << std::endl;
    //             counter++;

    //             // random node from 0 to numNodes
    //             int totalNodes = branch->getNumNodes();
    //             randPercent = (float)rand() / (float)RAND_MAX;
    //             randPercent = randPercent * 0.8 + 0.1;
    //             int randomNode = (int)(randPercent * totalNodes);

    //             TreeBranch* newBranch = treeManager->addBranch(branch->getID(), randomNode);
    //             newBranch->pushBackTexture(branchDiffuse);
    //             newBranch->pushBackTexture(branchNormal);
    //             newBranch->addShader(FRAME_BUFFER, "tree");
    //             newBranch->addShader(SHADOW_BUFFER, "treeShadowMap");
    //             newBranch->getLeafManager()->addShader(FRAME_BUFFER, "leaf");
    //             newBranch->getLeafManager()->addShader(SHADOW_BUFFER, "leafShadowMap");
    //             for (int i = 0; i < numNodes; i++)
    //                 newBranch->addNode(entry);
    //             newBranch->recalculateVertices();

    //             // for( int i = 0; i < newBranch->getNumNodes(); ){
    //             //     TreeVertex* vertex = newBranch->getNode(i)->getVertexData();
    //             //     LeafKey key = newBranch->getLeafManager()->getNewLeafKey();
    //             //     newBranch->getLeafManager()->writeLeafData(key, vertex->position, vertex->direction);
    //             //     // i += rand() % 3 + 1;
    //             //     i++;
    //             // }
    //             // newBranch->getLeafManager()->writeDataToGPU();

    //             if (depth - 1 > 0){
    //                 // std::cout << "PUSH " << (depth - 1) << std::endl;
    //                 branchStack.push({newBranch, depth - 1});
    //             }
    //         }
    //     }
    // }

    Gameobject* treeManager = new Gameobject("Tree Manager");
    treeManager->addComponent(this->userTree);
    treeManager->addComponent(new TreeRendererComponent(this->userTree->getTreeManager()));
    treeManager->setPosition(glm::vec3(worldSize/2, 280, worldSize/2));
    treeManager->setScale(glm::vec3(100.0f));
    // treeManager->setScale(glm::vec3(300.0f));
    addGameobject(treeManager);

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
    uiCanvas = new UICanvas();
    ui->addComponent(uiCanvas);
    ui->active = false;
    addGameobject(ui);

    Texture* texture = new Texture("../resources/textures/UI/Paper.png", TextureType::Diffuse);
    Image* imageMesh = new Image(texture, glm::vec2(0.05, 0.05), glm::vec2(0.95, 0.95), glm::vec3(1.0f, 1.0f, 1.0f));
    imageMesh->addShader(FRAME_BUFFER, "image");
    uiCanvas->addUIRenderObject(imageMesh);

    // Text* text = new Text(font, "", glm::vec2(0.1, 0.15), glm::vec2(0.9, 0.85), 0.8f, glm::vec3(0.0f, 0.0f, 0.0f));

    Text* headerText = new Text(font, "", glm::vec2(0.1, 0.85), glm::vec2(0.9, 0.90), 1.2f, glm::vec3(0.0f, 0.0f, 0.0f));
    headerText->setAlignment(TextAlignment::UPPER_LEFT);
    headerInputField = new InputField(headerText, "Header Text");
    headerInputField->getRenderObject()->addShader(FRAME_BUFFER, "text");
    uiCanvas->addUIComponent(headerInputField);


    Text* text = new Text(font, "", glm::vec2(0.1, 0.15), glm::vec2(0.9, 0.85), 0.8f, glm::vec3(0.0f, 0.0f, 0.0f));
    text->setAlignment(TextAlignment::UPPER_LEFT);
    inputField = new InputField(text, "Body Text");
    inputField->getRenderObject()->addShader(FRAME_BUFFER, "text");
    uiCanvas->addUIComponent(inputField);


    Text* text2 = new Text(font, steamUsername, glm::vec2(0.0, 0.0), glm::vec2(0.8, 0.25), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    text2->addShader(FRAME_BUFFER, "text");
    uiCanvas->addUIRenderObject(text2);

    Audio::instance.play("summer");

    // std::string result = userTree->addEntry(
    //     getCurrentDateTime(),
    //     "This is a test header",
    //     "This is a test entry"
    // );
    // Logger::log(result);

}

void TheLonelyTree::update(){
    if (Input::getKeyDown(KeyCode::KEY_F10)){
        Audio::instance.play("test");
    }

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
        lockCursor(true);
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
        lockCursor(false);

        if (Input::getKeyDown(KeyCode::KEY_F10)){
            std::string result = userTree->addEntry(
                getCurrentDateTime(),
                headerInputField->getTextObject()->getText(),
                inputField->getTextObject()->getText()
            );
            Logger::log(result);
        }
    }

    if (Input::getKeyUp(KeyCode::KEY_ESCAPE)){
        ui->active = !ui->active;
    }

    // std::cout << "FPS: " << FPS() << std::endl;
}

void TheLonelyTree::lateUpdate() {}