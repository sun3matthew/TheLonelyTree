#include "tree_manager.h"
#include "tree_node.h"
#include <iostream>
#include <random>
#include <cassert>


TreeManager::TreeManager(){
    unsigned int id = generateRandomID();
    tree[id] = new TreeBranch(id, nullptr, nullptr);

    rootBranchID = id;
}

TreeManager::~TreeManager(){
    for(auto branch : tree){
        deleteBranch(branch.first);
    }
}

unsigned int TreeManager::generateRandomID(){
    static std::mt19937 rng(std::random_device{}()); // Random number generator
    static std::uniform_int_distribution<unsigned int> dist(0, UINT_MAX);
    return dist(rng);
}

TreeBranch* TreeManager::addBranch(TreeNode* node){
    TreeBranch* nodeBranch = node->getAssociatedBranch();
    assert(nodeBranch != nullptr);

    // int id = generateRandomID();
    // int id = rand();
    int id = rand();
    TreeBranch* newBranch = new TreeBranch(id, nodeBranch, node);
    tree[id] = newBranch;

    return newBranch;
}

TreeBranch* TreeManager::addBranch(unsigned int branchId, int nodeIndex){
    TreeBranch* parentBranch = tree[branchId];
    assert(parentBranch != nullptr);

    TreeNode* node = parentBranch->getNode(nodeIndex);
    assert(node != nullptr);

    return addBranch(node);
}

void TreeManager::deleteBranch(unsigned int id){
    TreeBranch* branch = tree[id];
    assert(branch != nullptr);

    branch->markForDeletion();
    delete branch;
}