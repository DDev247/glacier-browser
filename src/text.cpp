
#include "../include/glfw.hpp"
#include "../include/text.h"
#include "../include/logger.h"

#include <ft2build.h>
#include <freetype/freetype.h>

glacier::CharacterMap glacier::Text::loadFont(std::string path, bool* failed, int height) {
	LOG(Log::Level::Debug, "glacier::Text::loadFont", "Loading font from path: " + path);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		LOG(Log::Level::Error, "glacier::Text::loadFont", "Could not initialise FreeType");
        *failed = true;
        return {};
	}

	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face)) {
		LOG(Log::Level::Error, "glacier::Text::loadFont", "Failed to load font");
        *failed = true;
        return {};
	}

	FT_Set_Pixel_Sizes(face, 0, height);

    CharacterMap chars = {};
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            LOG(Log::Level::Error, "glacier::Text::loadFont", "Failed to load glyph " + std::to_string((unsigned int)c));
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x,
            height
        };
        chars.insert(std::pair<char, Character>(c, character));
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);

    LOG(Log::Level::Debug, "glacier::Text::loadFont", "Loading done, cleaning FreeType");
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    *failed = false;
    return chars;
}
