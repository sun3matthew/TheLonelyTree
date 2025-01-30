#ifndef TREE_H
#define TREE_H


// Procedural Tree Generator

#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include "tree_node.h"
#include "tree_branch.h"

class TreeManager{
    public:
        TreeManager();
        ~TreeManager();

        TreeBranch* addBranch(TreeNode* node);
        TreeBranch* addBranch(unsigned int branchId, int nodeIndex);
        void deleteBranch(unsigned int id);
        
        TreeBranch* rootBranch(){ return tree[rootBranchID]; }

        unsigned int generateRandomID();

        std::unordered_map<unsigned int, TreeBranch*> tree;
        unsigned int rootBranchID;
};

#endif // TREE_H
