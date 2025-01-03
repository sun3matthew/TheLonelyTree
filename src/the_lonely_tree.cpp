#include <the_lonely_tree.h>

#include <iostream>
#include <engine/input.h>

TheLonelyTree::~TheLonelyTree(){

}

void TheLonelyTree::start(){
    createWindow(800, 600, "The Lonely Tree");
}

void TheLonelyTree::update(){

    if (Input::getKeyDown(KeyCode::KEY_A)){
        std::cout << "A Down" << std::endl;
    }

    if (Input::getKey(KeyCode::KEY_A)){
        std::cout << "A" << std::endl;
    }


    if (Input::getKeyUp(KeyCode::KEY_A)){
        std::cout << "A Up" << std::endl;
    }

    if (Input::getMouseDown(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button Down" << std::endl;
    }

    if (Input::getMouse(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button" << std::endl;
    }


    if (Input::getMouseUp(MouseButtonCode::MOUSE_BUTTON_LEFT)){
        std::cout << "Left Mouse Button Up" << std::endl;
    }

    if (Input::getKey(KeyCode::KEY_SPACE)){
        std::cout << "Mouse Position: " << Input::getMousePosition().x << ", " << Input::getMousePosition().y << std::endl;
        std::cout << "Mouse Delta: " << Input::getMouseDelta().x << ", " << Input::getMouseDelta().y << std::endl;

        std::cout << "Mouse Scroll: " << Input::getMouseScroll() << std::endl;
        std::cout << "Mouse Scroll X: " << Input::getMouseScrollX() << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

    // std::cout << "FPS: " << FPS() << std::endl;



}