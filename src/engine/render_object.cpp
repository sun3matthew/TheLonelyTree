#include <engine/render_object.h>
#include <engine/render_manager.h>
#include <cassert>

RenderObject::RenderObject(){}

void RenderObject::addShader(std::string shaderName){
    shaderNames.push_back(shaderName);
}

void RenderObject::draw(){
    for(std::string shaderName : shaderNames){
        drawCall(RenderManager::instance.getShader(shaderName));
    }
}