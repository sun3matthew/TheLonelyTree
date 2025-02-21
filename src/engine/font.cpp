
#include <engine/font.h>

#include <glad/glad.h>

#include <iostream>

// !!! CODE OF SHAME
// assert(!FT_Init_FreeType(&ft));
// assert(!FT_New_Face(ft, filepath.c_str(), 0, &face));
// !!! Asserts are not included in release

Font::Font(std::string filepath, int size)
{
    FT_Face face;
    FT_Library ft;

    bool success = FT_Init_FreeType(&ft);
    if (success != 0)
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    success = FT_New_Face(ft, filepath.c_str(), 0, &face);
    if (success != 0)
    {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        assert(!FT_Load_Char(face, c, FT_LOAD_RENDER));

        // generate texture
        Texture* texture = new Texture(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, TextureType::Font, false);

        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font(){
    for (std::pair<char, Character> character : Characters){
        delete character.second.texture;
    }
}
