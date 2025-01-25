#include <engine/render_manager.h>

#include <glad/glad.h> 


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

void RenderManager::addFrameBuffer(FrameBuffer frameBuffer){
    renderOrder.insert({frameBuffer.name, frameBuffers.size()});

    frameBuffers.push_back(frameBuffer);
    renderBuffer.push_back(std::vector<RenderObject*>());
}

void RenderManager::updateFrameBuffer(int i, int width, int height){
    frameBuffers[i].width = width;
    frameBuffers[i].height = height;
}

FrameBuffer RenderManager::getFrameBuffer(int i){
    return frameBuffers[i];
}
FrameBuffer RenderManager::getFrameBuffer(std::string layerName){
    return frameBuffers[renderOrder[layerName]];
}

void RenderManager::activateFrameBuffer(int i){
    FrameBuffer frameBuffer = frameBuffers[i];
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.framebuffer);
    glViewport(0, 0, frameBuffer.width, frameBuffer.height);
    glClear(frameBuffer.clearFlags);
}

void RenderManager::addToBuffer(std::string layer, RenderObject* renderObject){
    assert(renderOrder.find(layer) != renderOrder.end());

    renderBuffer[renderOrder[layer]].push_back(renderObject);
}

void RenderManager::draw(int i){
    activateFrameBuffer(i);
    for(RenderObject* renderObject : renderBuffer[i])
        renderObject->draw(frameBuffers[i].name);
}

void RenderManager::draw(std::string layer){
    assert(renderOrder.find(layer) != renderOrder.end());

    draw(renderOrder[layer]);
}

void RenderManager::addToBuffer(RenderObject* renderObject){
    addToBuffer(0, renderObject);
}
void RenderManager::draw(){
    for(int i = renderBuffer.size() - 1; i >= 0; i--){
        draw(i);
    }

    clearBuffer();
}


void RenderManager::clearBuffer(){
    for(int i = 0; i < renderBuffer.size(); i++){
        renderBuffer[i].clear();
    }
}