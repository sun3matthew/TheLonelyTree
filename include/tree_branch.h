#ifndef TREE_BRANCH_H
#define TREE_BRANCH_H

#include <vector>
#include <engine/render_object.h>
#include "tree_node.h"

class TreeBranch : public RenderObject{
public:
    TreeBranch(unsigned int ID, TreeBranch* parentBranch, TreeNode* node);
    ~TreeBranch();

    TreeBranch* getParentBranch();
    TreeNode* getRootNode();

    int getNumNodes();
    TreeNode* getNode(int idx);
    void addNode(glm::vec3 direction, float magnitude, Entry entry);

    unsigned int getID();
    void markForDeletion();

    void drawCall(Shader* shader) override;
    void writeDataToGPU();
private:
    unsigned int ID;
    unsigned int VAO, VBO;

    TreeBranch* parentBranch;
    TreeNode* rootNode;
    std::vector<TreeNode*> nodes;
    std::vector<TreeVertex> vertices;
};


#endif
