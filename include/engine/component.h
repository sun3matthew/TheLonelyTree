#ifndef COMPONENT_H
#define COMPONENT_H

class Gameobject;

class Component{
    public:
        bool active;

        Component();
        virtual ~Component(); // Should only take care of cleaning itself

        virtual void update() = 0;
        virtual void lateUpdate();

        Gameobject* getGameobject();
        void setGameobject(Gameobject*);
    private:
        Gameobject* gameobject;
};

#endif