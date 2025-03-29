
#include "tree_manager.h"
#include "tree_node.h"
#include "user_tree.h"

#include <iostream>
#include <random>
#include <cassert>

TreeManager::TreeManager(unsigned long long id) : rootBranchID(id){
}

TreeManager::~TreeManager(){
    for(auto branch : tree){
        deleteBranch(branch.first);
    }
}

TreeBranch* TreeManager::addBranch(unsigned long long branchId, unsigned long long parentBranchID, std::string nodeKey){
    TreeBranch* parentBranch = tree[parentBranchID];
    if (parentBranch == nullptr){
        TreeBranch* newBranch = new TreeBranch(branchId, nullptr, nullptr, new LeafManager());
        tree[branchId] = newBranch;
        return newBranch;
    }

    // TODO optimize
    int nodeCount = parentBranch->getNumNodes();
    for(int i = 0; i < nodeCount; i++){
        if (parentBranch->getNode(i)->getEntry().getKey() == nodeKey){
            TreeBranch* newBranch = new TreeBranch(branchId, parentBranch, parentBranch->getNode(i), new LeafManager());
            tree[branchId] = newBranch;
            return newBranch;
        }
    }

    return nullptr;
}

void TreeManager::deleteBranch(unsigned int id){
    TreeBranch* branch = tree[id];
    if (branch == nullptr){
        return;
    }

    branch->markForDeletion();
    delete branch;
}