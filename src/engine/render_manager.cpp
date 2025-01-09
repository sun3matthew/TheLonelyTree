#include <engine/render_manager.h>

// TODO Free this at game end.
RenderManager RenderManager::instance;

RenderManager::RenderManager(){
}

void RenderManager::addShader(Shader* shader){
    shaders.insert({shader->getName(), shader});
}

RenderManager::~RenderManager(){
    for (const auto& [key, value] : shaders) {
        delete value;
    }
}

std::vector<Shader*> RenderManager::getShadersAccepting(std::string attribute){
    std::vector<Shader*> acceptingShaders;
    for (const auto& [key, value] : shaders) {
        if(value->canAcceptAttribute(attribute))
            acceptingShaders.push_back(value);
    }
    return acceptingShaders;
}

Shader* RenderManager::getShader(std::string name){
    return shaders[name];
}


void RenderManager::addToBuffer(RenderObject* renderObject){
    renderBuffer.push_back(renderObject);
}

// Draws and also empties buffer.
void RenderManager::draw(){
    for(RenderObject* renderObject : renderBuffer)
        renderObject->draw();
    
    renderBuffer.clear();
}