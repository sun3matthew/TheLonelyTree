#ifndef TREE_NODE_H
#define TREE_NODE_H


#include <vector>
#include <glm/glm.hpp>
class TreeBranch;

struct TreeVertex{
    glm::vec3 position;
    glm::vec3 direction;

    TreeVertex(){
        position = glm::vec3(0.0f);
        direction = glm::vec3(0.0f);
    }
};

struct Entry{
    std::string date;
    std::string name;
    std::string data;
};

class TreeNode{
    public:
        TreeNode(TreeBranch* associatedBranch, TreeVertex* vertex, glm::vec3 position, glm::vec3 direction, Entry entry);
        ~TreeNode();

        void updatePosition(glm::vec3 position);
        void updateDirection(glm::vec3 direction);

        TreeBranch* getAssociatedBranch();
        void recalculateVertex(TreeNode* parent);
    private:
        TreeBranch* associatedBranch;
        TreeVertex* vertex;

        glm::vec3 localPosition;
        glm::vec3 localDirection;

        Entry entry;
};

#endif // TREE_NODE_H

