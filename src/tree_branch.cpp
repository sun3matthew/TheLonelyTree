#include <glad/glad.h>

#include "tree_branch.h"
#include <engine/render_manager.h>


#include <cassert>

TreeBranch::TreeBranch(unsigned int id, TreeBranch* parentBranch, TreeNode* node){
    this->ID = id;

    this->parentBranch = parentBranch;
    this->rootNode = node;


    textures.push_back(Texture("../resources/textures/tree/Diffuse.jpeg", TextureType::Diffuse));


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);

    writeDataToGPU();

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, direction));
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, parentPosition));
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, parentDirection));

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

#include <iostream>
void TreeBranch::writeDataToGPU(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TreeVertex), &vertices[0], GL_STATIC_DRAW);  

    // for(auto vertex : vertices){
    //     std::cout << "Vertex: " << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << std::endl;
    // }
}


void TreeBranch::drawCall(Shader* shader){
    shader->setMat4("model", modelMatrix);
    // shader->setFloat("time", time);
    // shader->setVec3("worldCenter", WorldGeneration::worldSize()/2.0, 0.0, WorldGeneration::worldSize()/2.0);

    // shader->setTexture(perlinLane, 0);

    shader->setTexture(&RenderManager::instance.getFrameBuffer(SHADOW_BUFFER).textures[0], 0);
    for (int i = 0; i < textures.size(); i++){
        shader->setTexture(&textures[i], i + 1);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}

TreeBranch* TreeBranch::getParentBranch(){ return parentBranch; }
TreeNode* TreeBranch::getRootNode(){ return rootNode; }
int TreeBranch::getNumNodes(){ return nodes.size(); }
TreeNode* TreeBranch::getNode(int idx){ 
    if(idx < 0 || idx >= nodes.size())
        return nullptr;
    return nodes[idx];
}

// ! Only safe for one scope/ non vertices
TreeVertex* TreeBranch::getVertex(int idx){ 
    if(idx < 0 || idx >= vertices.size())
        return nullptr;
    return &vertices[idx];
}
void TreeBranch::addNode(glm::vec3 direction, float magnitude, Entry entry){
    vertices.push_back(TreeVertex());
    nodes.push_back(new TreeNode(this, vertices.size() - 1, direction, magnitude, entry));
    writeDataToGPU();
}

uint TreeBranch::getID(){ return ID; }
void TreeBranch::markForDeletion(){ ID = 0; }