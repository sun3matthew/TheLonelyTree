#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <list>
#include <string>

#include <engine/component.h>
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

        void setParent(Gameobject*);

        void update();
        void lateUpdate();
    private:

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