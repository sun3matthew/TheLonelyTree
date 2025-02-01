#include <tree_node.h>
#include <tree_branch.h>
#include <cassert>

#include <iostream>
TreeNode::TreeNode(TreeBranch* associatedBranch, int idx, glm::vec3 direction, float magnitude, Entry entry){
    this->associatedBranch = associatedBranch;
    this->index = idx;

    this->localDirection = glm::normalize(direction);
    this->magnitude = magnitude;

    this->entry = entry;

    if(idx == 0){
        if (associatedBranch->getParentBranch() == nullptr){
            recalculateVertex(nullptr);
        }else{
            recalculateVertex(associatedBranch->getRootNode());
        }
    }else{
        recalculateVertex(associatedBranch->getNode(associatedBranch->getNumNodes() - 1));
    }
}

TreeNode::~TreeNode(){
    assert(associatedBranch->getID() == 0);
}

void TreeNode::recalculateVertex(TreeNode* parent){
    TreeVertex* vertex = associatedBranch->getVertex(index);
    if (parent == nullptr){
        vertex->parentDirection = glm::vec3(0, 1, 0) * magnitude;
        vertex->parentPosition = glm::vec3(0.0f);
    }else{
        TreeVertex* parentVertex = parent->associatedBranch->getVertex(parent->index);

        vertex->parentDirection = parentVertex->direction;
        vertex->parentPosition = parentVertex->position;
    }

    vertex->direction = glm::normalize(vertex->parentDirection + localDirection * magnitude) * magnitude;
    vertex->position = vertex->parentPosition + vertex->direction;
}

void TreeNode::updateDirection(glm::vec3 direction){
    localDirection = glm::normalize(direction);
}

void TreeNode::updateMagnitude(float magnitude){
    this->magnitude = magnitude;
}

TreeBranch* TreeNode::getAssociatedBranch(){
    return associatedBranch;
}
