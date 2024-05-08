
#pragma once

#include <glm/vec2.hpp>
#include <map>
#include <string>

namespace glacier {

    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        long Advance;            // Offset to advance to next glyph
        int LoadedSize;          // Size of font loaded
    };

    typedef std::map<char, Character> CharacterMap;

	class Text {
	public:
		static CharacterMap loadFont(std::string path, bool* failed, int height = 48);
	};

}
