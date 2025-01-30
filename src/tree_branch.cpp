#include <glad/glad.h>

#include "tree_branch.h"

#include <cassert>

TreeBranch::TreeBranch(unsigned int id, TreeBranch* parentBranch, TreeNode* node){
    this->ID = id;

    this->parentBranch = parentBranch;
    this->rootNode = node;


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);

    writeDataToGPU();

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)0);

    glBindVertexArray(0);
}

TreeBranch::~TreeBranch(){
    assert(ID == 0);

    for(auto node : nodes){
        delete node;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TreeBranch::writeDataToGPU(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TreeVertex), &vertices[0], GL_STATIC_DRAW);  
}

#include <iostream>

void TreeBranch::drawCall(Shader* shader){
    // shader->setFloat("time", time);
    // shader->setVec3("worldCenter", WorldGeneration::worldSize()/2.0, 0.0, WorldGeneration::worldSize()/2.0);

    // shader->setTexture(perlinLane, 0);
    // shader->setTexture(&RenderManager::instance.getFrameBuffer(SHADOW_BUFFER).textures[0], 1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}

TreeBranch* TreeBranch::getParentBranch(){ return parentBranch; }
TreeNode* TreeBranch::getRootNode(){ return rootNode; }
int TreeBranch::getNumNodes(){ return nodes.size(); }
TreeNode* TreeBranch::getNode(int idx){ return nodes[idx]; }
void TreeBranch::addNode(glm::vec3 position, glm::vec3 direction, Entry entry){
    vertices.push_back(TreeVertex());
    nodes.push_back(new TreeNode(this, &vertices.back(), position, direction, entry));
    writeDataToGPU();
}

uint TreeBranch::getID(){ return ID; }
void TreeBranch::markForDeletion(){ ID = 0; }