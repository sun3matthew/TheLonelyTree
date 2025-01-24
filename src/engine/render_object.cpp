#include <engine/render_object.h>
#include <engine/render_manager.h>
#include <cassert>

RenderObject::RenderObject(){}

void RenderObject::addShader(std::string shaderName){
    shaderNames[0].push_back(shaderName);
}
void RenderObject::addShader(int i, std::string shaderName){
    shaderNames[i].push_back(shaderName);
}

void RenderObject::draw(int i){
    assert(shaderNames.find(i) != shaderNames.end());
    for(std::string shaderName : shaderNames[i]){
        drawCall(RenderManager::instance.getShader(shaderName));
    }
}