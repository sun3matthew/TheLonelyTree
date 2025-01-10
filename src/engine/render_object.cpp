#include <engine/render_object.h>
#include <engine/render_manager.h>
#include <cassert>

RenderObject::RenderObject(std::string name)
    : shaderName(name)
{
    assert(RenderManager::instance.getShader(name));
}

void RenderObject::draw(){
    drawCall(RenderManager::instance.getShader(shaderName));
}