#include <glad/glad.h>

#include "tree_branch.h"
#include <engine/render_manager.h>

#include <random>
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

    glEnableVertexAttribArray(4);	
    glVertexAttribPointer(4, 1, GL_INT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, depth));

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

void TreeBranch::recalculateVertices(){
    //TODO have a ID that specifies which algorithm it uses just in case we need to make a new algo

    std::mt19937 rng(ID);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<PointDirection> keyPoints;

    const float density = 0.1; // 10 nodes per unit
    const float maxVector = 10;
    const float r = 0.9;
    const float growthStageSize = 20;

    int maxKeyPoints = nodes.size() * density + 1;

    glm::vec3 currentPosition(0.0f);
    for(int i = 0; i < maxKeyPoints; i++){
        float completion = nodes.size()/(growthStageSize * i);
        if (completion > 1)
            completion = 1;

        float theta = dist(rng) * 2 * M_PI;
        float magnitude = dist(rng) * maxVector * pow(r, i) * completion;

        float x = cos(theta);
        float z = sin(theta);
        float y = (dist(rng) - 0.1);

        glm::vec3 direction = glm::normalize(glm::vec3(x, y, z));
        direction *= magnitude;

        keyPoints.push_back(PointDirection{currentPosition, direction});
        currentPosition += direction;
    }

    for(int i = 0; i < nodes.size(); i++){
        float tFull = i * density;
        int idx = (int)tFull;

        glm::vec3 p1 = keyPoints[idx].point;
        glm::vec3 p2 = keyPoints[idx].point + keyPoints[idx].direction;
        glm::vec3 p3 = keyPoints[idx + 1].point - keyPoints[idx + 1].direction; // reverse direction
        glm::vec3 p4 = keyPoints[idx + 1].point;

        // bezier curve & derivative
        float t = tFull - idx;
        glm::vec3 position = (float)pow(1 - t, 3) * p1 + 3 * t * (float)pow(1 - t, 2) * p2 + 3 * (float)pow(t, 2) * (1 - t) * p3 + (float)pow(t, 3) * p4;
        glm::vec3 direction = -3 * (float)pow(1 - t, 2) * p1 + 3.0f * p2 * (3 * t * t - 4 * t + 1) + 3.0f * p3 * (2 * t - 3 * t * t) - 3.0f * p4 * t * t;

        nodes[i]->setVertexData(i == 0 ? nullptr : nodes[i - 1], position, direction);
    }
    writeDataToGPU();
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
void TreeBranch::addNode(Entry entry){
    vertices.push_back(TreeVertex());
    nodes.push_back(new TreeNode(this, vertices.size() - 1, entry));
    writeDataToGPU();
}

uint TreeBranch::getID(){ return ID; }
void TreeBranch::markForDeletion(){ ID = 0; }