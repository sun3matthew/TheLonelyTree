#include <engine/render_object.h>
#include <engine/render_manager.h>
#include <cassert>


RenderObject::RenderObject(){}

void RenderObject::addShader(std::string shaderName){
    shaderNames[BASE_FAME_BUFFER].push_back(shaderName);
}
void RenderObject::addShader(std::string frameBufferName, std::string shaderName){
    shaderNames[frameBufferName].push_back(shaderName);
}

void RenderObject::draw(std::string frameBufferName){
    assert(shaderNames.find(frameBufferName) != shaderNames.end());
    for(std::string shaderName : shaderNames[frameBufferName]){
        Shader* shader = RenderManager::instance.getShader(shaderName);
        shader->use();
        drawCall(shader);
    }
}