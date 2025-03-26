#include <engine/gameobject.h>
#include "tree_renderer_component.h"
#include <engine/render_manager.h>

#define MAX_ITERATION 10000

TreeRendererComponent::TreeRendererComponent(TreeManager *treeManager){
    this->treeManager = treeManager;
}

TreeRendererComponent::~TreeRendererComponent(){
}

void TreeRendererComponent::update(){
    glm::mat4 modelMatrix = gameobject->getModelMatrix();

    treeManager->rootBranch()->modelMatrix = modelMatrix * treeManager->rootBranch()->getLocalModelMatrix();

    std::stack <TreeBranch*> branchStack;
    branchStack.push(treeManager->rootBranch());

    int failSafe = 0;
    while(!branchStack.empty()){
        TreeBranch* branch = branchStack.top();
        branchStack.pop();

        for(TreeBranch* childBranch : branch->getChildBranches()){
            childBranch->modelMatrix = branch->modelMatrix * childBranch->getLocalModelMatrix();
            childBranch->getLeafManager()->modelMatrix = childBranch->modelMatrix;
            branchStack.push(childBranch);
        }

        for(auto& [i, shaderNames] : branch->shaderNames){
            RenderManager::instance.addToBuffer(i, branch);
            RenderManager::instance.addToBuffer(i, branch->getLeafManager());
        }

        if (failSafe++ > MAX_ITERATION){
            assert(false);
            break;
        }
    }
}