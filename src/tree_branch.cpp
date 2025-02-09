#include <glad/glad.h>

#include "tree_branch.h"
#include <engine/render_manager.h>

#include <random>
#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

TreeBranch::TreeBranch(unsigned int id, TreeBranch* parentBranch, TreeNode* node, LeafManager* leafManager){
    this->depth = 1;
    this->ID = id;
    this->rootNode = node;

    this->leafManager = new LeafManager();

    this->parentBranch = parentBranch;
    if (parentBranch){
        parentBranch->childBranches.push_back(this);
        this->depth = parentBranch->depth + 1;
    }

    localModelMatrix = glm::mat4(1.0f); // ! important.

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);

    // Initialize buffers
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
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, radius));
    glEnableVertexAttribArray(5);	
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, parentRadius));

    glBindVertexArray(0);
}

TreeBranch::~TreeBranch(){
    assert(ID == 0);

    delete leafManager;

    for(auto node : nodes){
        delete node;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

LeafManager* TreeBranch::getLeafManager(){
    return leafManager;
}

int TreeBranch::getDepth(){
    return depth;
}

void TreeBranch::pushBackTexture(Texture texture){
    textures.push_back(texture);
}

glm::mat4 TreeBranch::getLocalModelMatrix(){
    return localModelMatrix;
}

std::vector<TreeBranch*> TreeBranch::getChildBranches(){
    return childBranches;
}

void TreeBranch::writeDataToGPU(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TreeVertex), &vertices[0], GL_STATIC_DRAW);  
}

glm::vec3 generateBranchDirection(glm::vec3 direction, float tiltAngle, float bearingAngle) {
    glm::vec3 dir = glm::normalize(direction);

    glm::vec3 arbitrary = (glm::abs(dir.x) < 0.9f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);

    glm::vec3 t = glm::normalize(glm::cross(dir, arbitrary));
    glm::vec3 b = glm::normalize(glm::cross(t, dir));

    float theta = glm::radians(tiltAngle);
    float phi = glm::radians(bearingAngle);

    return glm::normalize(
        dir * glm::cos(theta) +
        t * glm::sin(theta) * glm::cos(phi) +
        b * glm::sin(theta) * glm::sin(phi)
    );
}

void TreeBranch::recalculateVertices(){
    //TODO have a ID that specifies which algorithm it uses just in case we need to make a new algo
    std::mt19937 rng(ID);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<PointDirection> keyPoints;

    const float density = 0.10; // 10 nodes per unit
    const float maxVector = 2;
    const float r = 0.9;
    const float growthStageSize = 10;

    int maxKeyPoints = nodes.size() * density + 2;


    glm::vec3 currentPosition(0.0f);
    glm::vec3 currentDirection(0, 1, 0);
    glm::vec3 branchDirection(0, 1, 0);
    int parentDepth = 0;
    if(rootNode){
        TreeVertex* rootVertex = rootNode->getAssociatedBranch()->getVertex(rootNode->getIndex());

        localModelMatrix = glm::translate(glm::mat4(1.0), rootVertex->position);
        // currentPosition = rootVertex->position; // TODO modify so this is applied in model matrix
        currentDirection = rootVertex->direction;
        branchDirection = rootVertex->direction;
    }
    // parentDepth += 10;

    for(int i = 0; i < maxKeyPoints; i++){
        float completion = nodes.size()/(growthStageSize * i);
        if (completion > 1)
            completion = 1;
        completion = 1;

        float theta = dist(rng) * 360;
        float tiltAngle = dist(rng) * 120;
        float magnitude = ((dist(rng) + 1) / 2.0f) * maxVector * pow(r, i) * completion;

        // float x = cos(theta);
        // float z = sin(theta);
        // float y = (dist(rng) + 0.4) * 1;
        // glm::vec3 direction = glm::normalize(glm::vec3(x, y, z));

        glm::vec3 direction = generateBranchDirection(branchDirection, tiltAngle, theta);

        direction = glm::normalize(glm::normalize(currentDirection) + direction);
        direction *= magnitude;

        keyPoints.push_back(PointDirection{currentPosition, direction});
        currentPosition += direction;
        currentDirection = direction;
    }

    for(int i = 0; i < nodes.size(); i++){
        float tFull = i * density;
        int idx = (int)tFull;

        glm::vec3 p1 = keyPoints[idx].point;
        glm::vec3 p2 = keyPoints[idx].point + keyPoints[idx].direction / 4.0f;
        glm::vec3 p3 = keyPoints[idx + 1].point - keyPoints[idx + 1].direction / 4.0f; // reverse direction
        glm::vec3 p4 = keyPoints[idx + 1].point;

        // bezier curve & derivative
        float t = tFull - idx;
        t += density/2.0;
        // t /= 2;
        // t = 0;
        glm::vec3 position = (float)pow(1 - t, 3) * p1 + 3 * t * (float)pow(1 - t, 2) * p2 + 3 * t * t * (1 - t) * p3 + (float)pow(t, 3) * p4;
        // glm::vec3 direction = -3 * (float)pow(1 - t, 2) * p1 + 3.0f * p2 * (3 * t * t - 4 * t + 1) + 3.0f * p3 * (2 * t - 3 * t * t) - 3.0f * p4 * t * t;

        // std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
        // std::cout << "Index: " << t << std::endl;

        nodes[i]->setVertexData(i == 0 ? nullptr : nodes[i - 1], position, (i + 1) / (float)nodes.size());
        // nodes[i]->setVertexData(i == 0 ? nullptr : nodes[i - 1], glm::vec3(10, i * 0.1, 10));
        // nodes[i]->setVertexData(i == 0 ? nullptr : nodes[i - 1], position, glm::vec3(0, 1, 0));
    }
    

    writeDataToGPU();
}


#include <iostream>
void TreeBranch::drawCall(Shader* shader){
    shader->setMat4("model", modelMatrix);

    shader->setInt("numNodes", nodes.size());
    shader->setInt("branchDepth", depth);

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
    // writeDataToGPU();
}

uint TreeBranch::getID(){ return ID; }
void TreeBranch::markForDeletion(){ ID = 0; }