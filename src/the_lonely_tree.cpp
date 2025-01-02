#include <the_lonely_tree.h>

#include <iostream>

TheLonelyTree::~TheLonelyTree(){

}

void TheLonelyTree::start(){
    createWindow(800, 600, "The Lonely Tree");
}

void TheLonelyTree::update(){



    std::cout << "TheLonelyTree Update" << std::endl;
}