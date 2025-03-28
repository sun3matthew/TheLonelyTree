#ifndef TREE_NODE_H
#define TREE_NODE_H


#include <vector>
#include <glm/glm.hpp>

#include "entry.h"
class TreeBranch;

struct TreeVertex{
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 parentPosition;
    glm::vec3 parentDirection;

    float radius; // better struct packing..?
    float parentRadius;
    TreeVertex(){
        position = glm::vec3(0.0f);
        direction = glm::vec3(0.0f);

        parentPosition = glm::vec3(0.0f);
        parentDirection = glm::vec3(0.0f);

        radius = 0.0f;
        parentRadius = 0.0f;
    }
};

class TreeNode{
    public:
        TreeNode(TreeBranch* associatedBranch, int idx, Entry entry);
        ~TreeNode();

        TreeBranch* getAssociatedBranch();
        void setVertexData(TreeNode* parent, glm::vec3 position, float nodePercent);
        TreeVertex* getVertexData();

        Entry& getEntry();
        int getIndex();

    private:
        Entry entry;

    TreeBranch* associatedBranch;
        int index;
};

#endif // TREE_NODE_H

