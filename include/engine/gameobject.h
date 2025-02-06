#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <list>
#include <vector>
#include <string>

#include <engine/component.h>
#include <glm/gtc/type_ptr.hpp>

// #include <cassert>

class Gameobject{
    public:
        bool active;
        std::string name;

        Gameobject();
        Gameobject(std::string);
        ~Gameobject();

        void addComponent(Component*);
        template <typename T>
        T* getComponent();
        template <typename T>
        std::vector<T*> getComponents();
        void removeComponent(Component*);

        std::list<Gameobject*> getChildren();
        std::list<Gameobject*> getAllChildren();

        void setParent(Gameobject*);
        void removeChild(Gameobject*);

        void update();
        void lateUpdate();

        glm::vec3 getPosition();
        void setPosition(glm::vec3);
        glm::quat getRotation();
        void setRotation(glm::quat);
        glm::vec3 getScale();
        void setScale(glm::vec3);
        glm::mat4 getModelMatrix();
    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::mat4 modelMatrix;
        void updateModelMatrix();

        //TODO investigate if you should use a unordered_map instead.
        Gameobject* parent;
        std::list<Gameobject*> children;

        std::list<Component*> components;
};



template <typename T>
T* Gameobject::getComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    for (Component* component : components) {
        T* derived = dynamic_cast<T*>(component);
        if (derived) {
            return derived;
        }
    }
    return nullptr; // Not found
}

template <typename T>
std::vector<T*> Gameobject::getComponents() {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    std::vector<T*> components;

    for (Component* component : components) {
        T* derived = dynamic_cast<T*>(component);
        if (derived) {
            components.push_back(derived);
        }
    }
    return components;
}


#endif