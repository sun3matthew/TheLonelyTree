#include <engine/render_manager.h>
#include <glad/glad.h>

#include "leaf_manager.h"

std::vector<Texture> LeafManager::leafTextures = {};

LeafManager::LeafManager(){
    if (LeafManager::leafTextures.size() == 0){
        LeafManager::leafTextures.push_back(Texture("../resources/textures/tree/leaves/Diffuse.png", TextureType::Diffuse));
        LeafManager::leafTextures.push_back(Texture("../resources/textures/tree/leaves/Normal.png", TextureType::Normal));
    }

    int numLeafTypes = LeafManager::leafTextures[0].getWidth() / LeafManager::leafTextures[0].getHeight();
    for (int i = 0; i < numLeafTypes; i++){
        LeafSet* leafSet = new LeafSet();
        
        glGenVertexArrays(1, &leafSet->VAO);
        glGenBuffers(1, &leafSet->VBO);
    
        glBindVertexArray(leafSet->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, leafSet->VBO);

        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Leaf), (void*)0);
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Leaf), (void*)offsetof(Leaf, direction));
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Leaf), (void*)offsetof(Leaf, type));

        glBindVertexArray(0); 

        leafSets.push_back(leafSet);
    }

}

LeafManager::~LeafManager(){
    for (LeafSet* leafSet : leafSets){
        glDeleteVertexArrays(1, &leafSet->VAO);
        delete leafSet;
    }
}

LeafKey LeafManager::getNewLeafKey(){
    int randomLeafSet = rand() % leafSets.size();
    LeafSet* leafSet = leafSets[randomLeafSet];
    
    LeafKey key;
    key.LeafType = randomLeafSet;
    key.LeafIdx = leafSet->leafData.size();
    leafSet->leafData.push_back(Leaf{glm::vec3(0.0f), glm::vec3(0, 1, 0), randomLeafSet});
    return key;
}

void LeafManager::writeLeafData(LeafKey key, glm::vec3 point, glm::vec3 direction){
    LeafSet* leafSet = leafSets[key.LeafType];
    leafSet->leafData[key.LeafIdx].point = point;
    leafSet->leafData[key.LeafIdx].direction = direction;
}

#include <iostream>
void LeafManager::drawCall(Shader* shader){
    shader->setMat4("model", modelMatrix);
    shader->setInt("numLeafTypes", leafSets.size());

    shader->setTexture(&RenderManager::instance.getFrameBuffer(SHADOW_BUFFER).textures[0], 0);
    for (int i = 0; i < LeafManager::leafTextures.size(); i++)
        shader->setTexture(&LeafManager::leafTextures[i], i + 1);

    for (LeafSet* leafSet : leafSets){
        glBindVertexArray(leafSet->VAO);
        glDrawArrays(GL_POINTS, 0, leafSet->leafData.size());
        glBindVertexArray(0);
    }
}

void LeafManager::writeDataToGPU(){
    for (LeafSet* leafSet : leafSets){
        glBindVertexArray(leafSet->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, leafSet->VBO);
        glBufferData(GL_ARRAY_BUFFER, leafSet->leafData.size() * sizeof(Leaf), &leafSet->leafData[0], GL_STATIC_DRAW);
    }
}