#include <tree_node.h>
#include <tree_branch.h>
#include <cassert>

TreeNode::TreeNode(TreeBranch* associatedBranch, TreeVertex* vertex, glm::vec3 direction, float magnitude, Entry entry){
    this->associatedBranch = associatedBranch;
    this->vertex = vertex;

    this->localDirection = glm::normalize(direction);
    this->magnitude = magnitude;

    this->entry = entry;

    //TODO if 0 then get attachment node from parent
    recalculateVertex(associatedBranch->getNode(associatedBranch->getNumNodes() - 1));
}

TreeNode::~TreeNode(){
    assert(associatedBranch->getID() == 0);
}

void TreeNode::recalculateVertex(TreeNode* parent){
    if (parent == nullptr){
        vertex->direction = localDirection;
        vertex->position = glm::vec3(0.0f);
        return;
    }
    vertex->direction = glm::normalize(parent->vertex->direction + localDirection);
    vertex->position = parent->vertex->position + vertex->direction * magnitude;
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
