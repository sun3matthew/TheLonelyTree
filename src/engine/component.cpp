
#include <engine/gameobject.h>
#include <cassert>

//! Never manually free a component
Component::Component() {
    gameobject = nullptr;
    active = true;
}

Component::~Component() {
}

void Component::lateUpdate() {}

Gameobject* Component::getGameobject(){
    return gameobject;
}

// !only gameobject should ever call this.
void Component::setGameobject(Gameobject* gameobject){
    assert(this->gameobject == nullptr);

    this->gameobject = gameobject;
}