#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>
#include <engine/gameobject.h>
#include <engine/camera.h>

#include <engine/font.h>
#include <engine/text.h>

#include <vector>
#include <list>

#include "tree_manager.h"

class TheLonelyTree : public GLFWWrapper{
    public:
        TheLonelyTree* instance;

        TheLonelyTree();
        ~TheLonelyTree() override;
    protected:
        void start() override;
        void update() override;
        void lateUpdate() override;

    private:
        Camera* camera;
        int numLights;

        TreeManager* treeManager;
        Font* font;

        Gameobject* ui;
        Gameobject* grass;
        Text* textMesh;

        std::string entry;
        std::string steamUsername;
};  

#endif