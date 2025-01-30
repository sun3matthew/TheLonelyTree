// class TreeNode{
//     public:
//         TreeNode(TreeBranch* associatedBranch, int treeVertexIndex, glm::vec3 position, glm::vec3 direction, Entry entry);
//         ~TreeNode();

//         void addChild(TreeNode* child);
//         void updatePosition(glm::vec3 position);
//         void updateDirection(glm::vec3 direction);

//         TreeBranch* getAssociatedBranch();
//         unsigned int getTreeVertexIndex();
//     private:
//         TreeBranch* associatedBranch;

//         glm::vec3 localPosition;
//         glm::vec3 localDirection;

//         unsigned int treeVertexIndex;
//         TreeVertex* vertex;

//         Entry entry;
// };

#include <tree_node.h>
#include <tree_branch.h>
#include <cassert>

TreeNode::TreeNode(TreeBranch* associatedBranch, TreeVertex* vertex, glm::vec3 position, glm::vec3 direction, Entry entry){
    this->associatedBranch = associatedBranch;
    this->vertex = vertex;

    this->localPosition = position;
    this->localDirection = direction;

    this->entry = entry;

    int idx = associatedBranch->getNumNodes() - 1;
    if (idx >= 0){
        //TODO if 0 then get attachment node from parent
        recalculateVertex(associatedBranch->getNode(associatedBranch->getNumNodes() - 1));
    }
}

TreeNode::~TreeNode(){
    assert(associatedBranch->getID() == 0);
}

void TreeNode::recalculateVertex(TreeNode* parent){
    vertex->position = parent->vertex->position + localPosition;
    vertex->direction = localDirection;
}

void TreeNode::updatePosition(glm::vec3 position){
    localPosition = position;
}

void TreeNode::updateDirection(glm::vec3 direction){
    localDirection = direction;
}

TreeBranch* TreeNode::getAssociatedBranch(){
    return associatedBranch;
}
