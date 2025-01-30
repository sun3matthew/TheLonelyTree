#ifndef TREE_RENDERER_COMPONENT_H
#define TREE_RENDERER_COMPONENT_H

#include <engine/component.h>
#include <engine/render_object.h>
#include <tree_manager.h>

class TreeRendererComponent : public Component{
    public:
        TreeRendererComponent(TreeManager *treeManager);
        ~TreeRendererComponent() override;

        void update() override;
    private:
        TreeManager* treeManager;
};

#endif