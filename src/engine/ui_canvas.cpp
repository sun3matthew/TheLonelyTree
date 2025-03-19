#include <engine/glfw_wrapper.h>

#include <engine/ui_canvas.h>
#include <engine/gameobject.h>
#include <engine/render_object_component.h>
#include <engine/input.h>

// This is a really *ss way of doing this.

UICanvas::UICanvas()
{
    currentFocused = nullptr;
}

UICanvas::~UICanvas()
{
}

// void addUIRenderObject(UIComponent* uiRenderObject);
// void removeUIRenderObject(UIComponent* uiRenderObject);

// UIComponent* getUIComponent(int index);
// UIComponent* getCurrentFocused();

#include <iostream>

void UICanvas::update()
{
    if (Input::getMouseDown(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        glm::vec2 mousePosition = Input::getMousePositionScreenSpace();
        bool clicked = false;
        for (int i = UIComponents.size() - 1; i >= 0; i--){
            UIComponent* uiComponent = UIComponents[i];
            AABB aabb = uiComponent->getUIRenderObject()->getAABB();
            if (aabb.contains(mousePosition) && uiComponent->isClickable()){
                if (currentFocused && currentFocused != uiComponent){
                    currentFocused->onUnfocused();
                }

                currentFocused = uiComponent;
                currentFocused->onClick();

                clicked = true;
                break;
            }
        }

        if (!clicked && currentFocused){
            currentFocused->onUnfocused();
            currentFocused = nullptr;
        }
    }

    if (currentFocused){
        currentFocused->focused();
    }
}

UIComponent* UICanvas::getUIComponent(int index){
    return UIComponents[index];
}

UIComponent* UICanvas::getCurrentFocused(){
    return currentFocused;
}

void UICanvas::addUIComponent(UIComponent* uiComponent){
    uiComponent->setCanvas(this);

    UIComponents.push_back(uiComponent);

    Gameobject* uiComponentGameobject = new Gameobject("UI Component");
    uiComponentGameobject->addComponent(uiComponent);
    uiComponentGameobject->setParent(gameobject);
}

void UICanvas::removeUIComponent(UIComponent* uiComponent){
    removeUIRenderObject(uiComponent->getUIRenderObject());
}

void UICanvas::addUIRenderObject(UIRenderObject* uiRenderObject){
    UIComponent* uiComponent = new UIComponent(uiRenderObject);
    addUIComponent(uiComponent);
}

void UICanvas::removeUIRenderObject(UIRenderObject* uiRenderObject){
    for (int i = 0; i < UIComponents.size(); i++){
        if (UIComponents[i]->getRenderObject() == uiRenderObject){
            UIComponents.erase(UIComponents.begin() + i);
            break;
        }
    }

    for (Gameobject* child : gameobject->getChildren()){
        RenderObjectComponent* renderObjectComponent = child->getComponent<RenderObjectComponent>();
        if (renderObjectComponent && renderObjectComponent->getRenderObject() == uiRenderObject){
            gameobject->removeChild(child);
            break;
        }
    }
}
