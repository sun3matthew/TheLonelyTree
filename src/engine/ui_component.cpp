#include <engine/ui_component.h>
#include <engine/ui_canvas.h>
#include <engine/input.h>

UIComponent::UIComponent(UIRenderObject* renderObject)
    : clickable(false), canvas(nullptr), RenderObjectComponent(renderObject){
}

UIComponent::~UIComponent()
{}

void UIComponent::setCanvas(UICanvas* canvas){
    this->canvas = canvas;
}

UIRenderObject* UIComponent::getUIRenderObject(){
    return static_cast<UIRenderObject*>(renderObject);
}

void UIComponent::update(){
    RenderObjectComponent::update();

    assert(canvas != nullptr);

    glm::vec2 mousePosition = Input::getMousePosition();
    AABB aabb = getUIRenderObject()->getAABB();
    hovering(aabb.contains(mousePosition));
}

void UIComponent::setClickable(bool clickable){
    this->clickable = clickable;
}

bool UIComponent::isClickable(){
    return clickable;
}

void UIComponent::onClick() {}
void UIComponent::hovering(bool hover) {}
void UIComponent::focused() {}
void UIComponent::onUnfocused() {}