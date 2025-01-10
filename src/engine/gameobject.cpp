
#include <engine/gameobject.h>

#include "glm/gtc/matrix_transform.hpp"


Gameobject::Gameobject()
    : Gameobject("")
{}

Gameobject::Gameobject(std::string gameobjectName)
    : parent(nullptr), name(std::move(gameobjectName)), active(true),
      position(glm::vec3(0.0f)), 
      rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), 
      scale(glm::vec3(1.0f)), 
      modelMatrix(glm::mat4(1.0f))
{}


Gameobject::~Gameobject(){
    for(Component* component : components){
        delete component;
    }
    components.clear();

    for(Gameobject* child : children){
        delete child;
    }
    children.clear();
}

void Gameobject::addComponent(Component* component){
    component->setGameobject(this);
    components.push_back(component);
}

// Removes and also frees component
void Gameobject::removeComponent(Component* component){
    components.remove(component);
    delete component;
}

void Gameobject::setParent(Gameobject* parent){
    if(this->parent){
        this->children.remove(this);
    }

    this->parent = parent;
    if(parent)
        parent->children.push_back(this);
}

glm::vec3 Gameobject::getPosition(){
    return position;
}

void Gameobject::setPosition(glm::vec3 position){
    this->position = position;
    updateModelMatrix();
}

glm::quat Gameobject::getRotation(){
    return rotation;
}

void Gameobject::setRotation(glm::quat rotation){
    this->rotation = rotation;
    updateModelMatrix();
}

glm::vec3 Gameobject::getScale(){
    return scale;
}

void Gameobject::setScale(glm::vec3 scale){
    this->scale = scale;
    updateModelMatrix();
}

glm::mat4 Gameobject::getModelMatrix(){
    return modelMatrix;
}

inline void Gameobject::updateModelMatrix(){
    if (parent){
        modelMatrix = parent->modelMatrix;
    }else{
        modelMatrix = glm::mat4(1.0f);
    }
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = modelMatrix * glm::mat4_cast(rotation);
    modelMatrix = glm::scale(modelMatrix, scale);

    for(Gameobject* child : children){
        child->updateModelMatrix();
    }
}

void Gameobject::update(){
    if(!active)
        return;

    for(Gameobject* child : children){
        child->update();
    }

    for(Component* component : components){
        if(component->active)
            component->update();
    }
}

void Gameobject::lateUpdate(){
    if(!active)
        return;

    for(Gameobject* child : children){
        child->lateUpdate();
    }

    for(Component* component : components){
        if(component->active)
            component->lateUpdate();
    }
}

