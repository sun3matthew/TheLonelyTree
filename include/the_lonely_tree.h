#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>
#include <engine/gameobject.h>
#include <engine/camera.h>

#include <engine/font.h>
#include <engine/text.h>

#include <vector>
#include <list>

#include "user_tree.h"

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

        bool screenShotMode = false;

        TreeManager* treeManager;
        UserTree* userTree;
        Font* font;

        Gameobject* ui;
        Gameobject* grass;
        Text* textMesh;

        std::string entry;
        std::string steamUsername;
        unsigned long long steamID;
};  

#endif