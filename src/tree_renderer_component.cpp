#include <engine/gameobject.h>
#include "tree_renderer_component.h"
#include <engine/render_manager.h>

TreeRendererComponent::TreeRendererComponent(TreeManager *treeManager){
    this->treeManager = treeManager;
}

TreeRendererComponent::~TreeRendererComponent(){
}

void TreeRendererComponent::update(){
    for(auto branch : treeManager->tree){
        TreeBranch* treeBranch = branch.second;
        treeBranch->modelMatrix = gameobject->getModelMatrix();
        for(auto& [i, shaderNames] : treeBranch->shaderNames){
            RenderManager::instance.addToBuffer(i, treeBranch);
        }
    }
}