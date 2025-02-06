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

    int depth;

    TreeVertex(){
        position = glm::vec3(0.0f);
        direction = glm::vec3(0.0f);

        parentPosition = glm::vec3(0.0f);
        parentDirection = glm::vec3(0.0f);

        depth = 0;
    }
};

struct Entry{
    std::string date;
    std::string name;
    std::string data;
};

class TreeNode{
    public:
        TreeNode(TreeBranch* associatedBranch, int idx, Entry entry);
        ~TreeNode();

        TreeBranch* getAssociatedBranch();
        void setVertexData(TreeNode* parent, int depth, glm::vec3 position);

        int getIndex();

        unsigned int HashedEntry();
    private:
        Entry entry;

        TreeBranch* associatedBranch;
        int index;
};

#endif // TREE_NODE_H

