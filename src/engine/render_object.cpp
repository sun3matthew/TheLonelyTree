#include <engine/render_object.h>
#include <engine/render_manager.h>

RenderObject::RenderObject(std::string name)
    : shaderName(name)
{}

void RenderObject::draw(){
    drawCall(RenderManager::instance.getShader(shaderName));
}