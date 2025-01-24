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
    frameBuffers.push_back(frameBuffer);
    renderBuffer.push_back(std::vector<RenderObject*>());
}

void RenderManager::updateFrameBuffer(int i, FrameBuffer frameBuffer){
    frameBuffers[i] = frameBuffer;
}

void RenderManager::activateFrameBuffer(int i){
    FrameBuffer frameBuffer = frameBuffers[i];
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.framebuffer);
    glViewport(0, 0, frameBuffer.width, frameBuffer.height);
    glClear(frameBuffer.clearFlags);
}

//! For frame buffers only, 0 takes default.
void RenderManager::addToBuffer(int i, RenderObject* renderObject){
    renderBuffer[i].push_back(renderObject);
}

void RenderManager::draw(int i){
    activateFrameBuffer(i);
    for(RenderObject* renderObject : renderBuffer[i])
        renderObject->draw(i);
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