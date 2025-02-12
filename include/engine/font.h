
#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <engine/texture.h>

#include <string>
#include <map>

struct Character {
    Texture* texture;
    glm::ivec2   Size;       
    glm::ivec2   Bearing;    
    unsigned int Advance;    
};

class Font
{
public:
    Font(std::string filepath, int size);
    ~Font();

    std::map<char, Character> Characters;
};

#endif