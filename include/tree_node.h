#ifndef TREE_NODE_H
#define TREE_NODE_H


#include <vector>
#include <glm/glm.hpp>
class TreeBranch;

struct TreeVertex{
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 parentPosition;
    glm::vec3 parentDirection;

    TreeVertex(){
        position = glm::vec3(0.0f);
        direction = glm::vec3(0.0f);

        parentPosition = glm::vec3(0.0f);
        parentDirection = glm::vec3(0.0f);
    }
};

struct Entry{
    std::string date;
    std::string name;
    std::string data;
};

class TreeNode{
    public:
        TreeNode(TreeBranch* associatedBranch, int idx, glm::vec3 direction, float magnitude, Entry entry);
        ~TreeNode();

        void updateMagnitude(float magnitude);
        void updateDirection(glm::vec3 direction);

        TreeBranch* getAssociatedBranch();
        void recalculateVertex(TreeNode* parent);

    private:
        Entry entry;

        TreeBranch* associatedBranch;
        int index;

        glm::vec3 localDirection;
        float magnitude;
};

#endif // TREE_NODE_H

