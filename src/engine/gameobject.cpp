
#include <engine/gameobject.h>

Gameobject::Gameobject()
    : parent(nullptr)
{
    active = true;
}

Gameobject::Gameobject(std::string gameobjectName)
    : parent(nullptr), name(gameobjectName)
{
    active = true;
}


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

