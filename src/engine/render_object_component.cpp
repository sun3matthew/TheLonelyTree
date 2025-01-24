#include <engine/render_object_component.h>
#include <engine/render_manager.h>
#include <engine/gameobject.h>

RenderObjectComponent::RenderObjectComponent(RenderObject* renderObjectIn)
    : renderObject(renderObjectIn)
{}

RenderObjectComponent::~RenderObjectComponent(){
    if(renderObject)
        delete renderObject;
}

void RenderObjectComponent::update(){
    renderObject->modelMatrix = gameobject->getModelMatrix();
    for(auto& [i, shaderNames] : renderObject->shaderNames){
        RenderManager::instance.addToBuffer(i, renderObject);
    }
}

void RenderObjectComponent::updateRenderObject(RenderObject* newRenderObject){
    if(renderObject)
        delete renderObject;
    renderObject = newRenderObject;
}

RenderObject* RenderObjectComponent::getRenderObject(){
    return renderObject;
}
