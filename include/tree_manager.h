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
        TreeManager(unsigned long long id);
        ~TreeManager();

        // void writeEntryData();
        // void writeBranchData();

        // TreeBranch* addBranch(TreeNode* node);
        // TreeBranch* addBranch(unsigned int branchId, int nodeIndex);

        TreeBranch* addBranch(unsigned long long branchId, unsigned long long parentBranchID, std::string nodeKey);

        void deleteBranch(unsigned int id);
        
        TreeBranch* rootBranch(){ return tree[rootBranchID]; }

        std::unordered_map<unsigned int, TreeBranch*> tree;
        unsigned long long rootBranchID;
};

#endif // TREE_H
