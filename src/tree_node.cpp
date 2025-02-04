#include <tree_node.h>
#include <tree_branch.h>
#include <cassert>

#include <iostream>
TreeNode::TreeNode(TreeBranch* associatedBranch, int idx, Entry entry){
    this->associatedBranch = associatedBranch;
    this->index = idx;

    this->entry = entry;
}

TreeNode::~TreeNode(){
    assert(associatedBranch->getID() == 0);
}

void TreeNode::setVertexData(TreeNode* parent, glm::vec3 position, glm::vec3 direction){
    TreeVertex* vertex = associatedBranch->getVertex(index);
    if (parent == nullptr){
        vertex->parentDirection = glm::vec3(0, 1, 0);
        vertex->parentPosition = glm::vec3(0.0f);
    }else{
        TreeVertex* parentVertex = parent->associatedBranch->getVertex(parent->index);

        vertex->parentDirection = parentVertex->direction;
        vertex->parentPosition = parentVertex->position;

        vertex->depth = parentVertex->depth + 1;
    }

    vertex->direction = glm::normalize(direction);
    vertex->position = vertex->parentPosition + vertex->direction;
}

TreeBranch* TreeNode::getAssociatedBranch(){
    return associatedBranch;
}
