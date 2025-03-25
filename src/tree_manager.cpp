
#include "tree_manager.h"
#include "tree_node.h"
#include "user_tree.h"

#include <iostream>
#include <random>
#include <cassert>

TreeManager::TreeManager(unsigned long long id) : rootBranchID(id){
}

// void TreeManager::writeEntryData(){
//     // EntryID,EntryID,EntryID,...
//     std::string data = "";
//     TreeBranch* rootBranch = tree[rootBranchID];
//     int count = rootBranch->getNumNodes();
//     for(int i = 0; i < count; i++){
//         data += rootBranch->getNode(i)->getEntry().getKey();
//         data += ",";
//     }

//     UserTree::instance->write(constructKey(EntryType::BranchIDEntryIDs, Crypto::toHex(rootBranchID)), data);
// }

// void TreeManager::writeBranchData(){
//     // ParentNode:NewID:{metadata},...,...
//     std::string data = "";
//     for(auto pair : tree){
//         TreeBranch* branch = pair.second;
//         TreeNode* root = branch->getRootNode();
//         if (root != nullptr && branch->getNumNodes() > 0){
//             data += root->getEntry().getKey();
//             data += ":";
//             data += std::to_string(branch->getNode(0)->getEntry().getCommitID());
//             data += ":";
//             data += std::to_string(0); // ! metadata
//             data += ",";
//         }
//     }

//     UserTree::instance->write(constructKey(EntryType::BranchIDBranchIDs, Crypto::toHex(rootBranchID)), data);
// }

TreeManager::~TreeManager(){
    for(auto branch : tree){
        deleteBranch(branch.first);
    }
}

// TreeBranch* TreeManager::addBranch(TreeNode* node){
//     TreeBranch* nodeBranch = node->getAssociatedBranch();
//     assert(nodeBranch != nullptr);

//     // int id = generateRandomID();
//     // int id = rand();
//     int id = rand();
//     TreeBranch* newBranch = new TreeBranch(id, nodeBranch, node, new LeafManager());
//     tree[id] = newBranch;

//     return newBranch;
// }

// TreeBranch* TreeManager::addBranch(unsigned int branchId, int nodeIndex){
//     TreeBranch* parentBranch = tree[branchId];
//     assert(parentBranch != nullptr);

//     TreeNode* node = parentBranch->getNode(nodeIndex);
//     assert(node != nullptr);

//     return addBranch(node);
// }

TreeBranch* TreeManager::addBranch(unsigned long long branchId, unsigned long long parentBranchID, std::string nodeKey){
    TreeBranch* parentBranch = tree[parentBranchID];
    if (parentBranch == nullptr){
        return new TreeBranch(branchId, nullptr, nullptr, new LeafManager());
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
    assert(branch != nullptr);

    branch->markForDeletion();
    delete branch;
}