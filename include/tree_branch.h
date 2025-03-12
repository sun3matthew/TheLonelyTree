#ifndef TREE_BRANCH_H
#define TREE_BRANCH_H

#include <vector>
#include <engine/render_object.h>
#include "tree_node.h"
#include "leaf_manager.h"

#include <engine/texture.h>

struct PointDirection{
    glm::vec3 point;
    glm::vec3 direction;
};

class TreeBranch : public RenderObject{
public:
    TreeBranch(unsigned long long ID, TreeBranch* parentBranch, TreeNode* node, LeafManager* leafManager);
    ~TreeBranch();

    LeafManager* getLeafManager();

    TreeBranch* getParentBranch();
    TreeNode* getRootNode();
    glm::mat4 getLocalModelMatrix();
    std::vector<TreeBranch*> getChildBranches();

    int getNumNodes();
    TreeNode* getNode(int idx);
    TreeVertex* getVertex(int idx);
    void addNode(Entry entry);

    int getDepth();
    unsigned long long getID();
    std::string getIDString();
    void markForDeletion();

    void drawCall(Shader* shader) override;
    void writeDataToGPU();
    void recalculateVertices();
    void pushBackTexture(Texture texture);

    std::string serializeNodes();
    std::string serializeChildBranches();
private:
    unsigned long long ID;
    std::string IDString;

    unsigned int VAO, VBO;

    int depth;

    LeafManager* leafManager;

    glm::mat4 localModelMatrix;

    TreeBranch* parentBranch;
    std::vector<TreeBranch*> childBranches;

    TreeNode* rootNode;

    std::vector<TreeNode*> nodes;
    std::vector<TreeVertex> vertices;

    std::vector<Texture>      textures;
};


#endif
