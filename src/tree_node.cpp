#include <tree_node.h>
#include <tree_branch.h>
#include <cassert>

#define RADIUS_C 0.5f

TreeNode::TreeNode(TreeBranch* associatedBranch, int idx, Entry entry){
    this->associatedBranch = associatedBranch;
    this->index = idx;

    this->entry = entry;
}

TreeNode::~TreeNode(){
    assert(associatedBranch->getID() == 0);
}

// radius
float calculateRadius(float x){
    return (1 / (1 - exp(-RADIUS_C))) * (exp(-RADIUS_C * x) - exp(-RADIUS_C));
}
// radius
#include <iostream>
void TreeNode::setVertexData(TreeNode* parent, glm::vec3 position, float nodePercent){

    TreeVertex* vertex = associatedBranch->getVertex(index);
    glm::vec3 direction = glm::vec3(0, 1, 0);

    float rootRadius = 1.0;
    if(associatedBranch->getRootNode()){
        rootRadius = associatedBranch->getRootNode()->getVertexData()->radius;
    }

    if (parent == nullptr){
        vertex->parentDirection = glm::vec3(0, 1, 0);
        vertex->parentPosition = glm::vec3(0.0f);
        vertex->parentRadius = rootRadius;
    }else{
        TreeVertex* parentVertex = parent->associatedBranch->getVertex(parent->index);

        vertex->parentDirection = parentVertex->direction;
        vertex->parentPosition = parentVertex->position;
        vertex->parentRadius = parentVertex->radius;

        direction = position - parentVertex->position;
    }

    // std::cout << nodePercent << std::endl;

    vertex->direction = glm::normalize(direction);
    vertex->position = position;
    vertex->radius = rootRadius * calculateRadius(nodePercent);
}

//! unsafe
TreeVertex* TreeNode::getVertexData(){
    return associatedBranch->getVertex(index);
}

int TreeNode::getIndex(){
    return index;
}

unsigned int TreeNode::HashedEntry(){
    unsigned int hash = 0;
    for (char c : entry.name){
        hash = hash * 31 + c;
    }
    return hash;
}

TreeBranch* TreeNode::getAssociatedBranch(){
    return associatedBranch;
}
