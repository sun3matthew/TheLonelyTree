#include <tree_node.h>
#include <tree_branch.h>
#include <cassert>

TreeNode::TreeNode(TreeBranch* associatedBranch, int idx, Entry entry){
    this->associatedBranch = associatedBranch;
    this->index = idx;

    this->entry = entry;
}

TreeNode::~TreeNode(){
    assert(associatedBranch->getID() == 0);
}

#include <iostream>
void TreeNode::setVertexData(TreeNode* parent, int depth, glm::vec3 position){
    TreeVertex* vertex = associatedBranch->getVertex(index);
    glm::vec3 direction = glm::vec3(0, 1, 0);
    if (parent == nullptr){
        vertex->parentDirection = glm::vec3(0, 1, 0);
        vertex->parentPosition = glm::vec3(0.0f);
    }else{
        TreeVertex* parentVertex = parent->associatedBranch->getVertex(parent->index);

        vertex->parentDirection = parentVertex->direction;
        vertex->parentPosition = parentVertex->position;


        direction = position - parentVertex->position;
    }

    vertex->depth = depth;
    vertex->direction = glm::normalize(direction);
    vertex->position = position;
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
