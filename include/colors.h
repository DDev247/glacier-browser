
#pragma once

#include <map>
#include <glm/vec4.hpp>

namespace glacier {

	enum ColorCode {
        // Colors
        Pink,
        Red,
        Maroon,
        Peach,
        Yellow,
        Green,
        Teal,
        Blue,
        Lavender,

        // Text shades
        Text,
        Subtext1,
        Subtext0,

        // Containers
        Overlay2,
        Overlay1,
        Overlay0,

        Surface2,
        Surface1,
        Surface0,

        // Backgrounds
        Base1,
        Base0
	};

	typedef std::map<ColorCode, glm::vec4> ColorPalette;

    class Colors {
    public:
        static glm::vec4 getColor(ColorCode code, int alpha = 255);
        static glm::vec4 getColorGL(ColorCode code, float alpha = 1.0f);
        static void setPalette(ColorPalette palette) { m_currentPalette = palette; };

    protected:
        static ColorPalette m_currentPalette;
    };

    /* Color palettes */

    // My own color palette (dark)
	static ColorPalette ddevDarkPalette = {
        { glacier::ColorCode::Pink,     { 0xd3, 0x99, 0xc7, 0xff } },
        { glacier::ColorCode::Red,      { 0xe9, 0x51, 0x5b, 0xff } },
        { glacier::ColorCode::Maroon,   { 0xdf, 0x6d, 0x74, 0xff } },
        { glacier::ColorCode::Peach,    { 0xf4, 0x84, 0x4d, 0xff } },
        { glacier::ColorCode::Yellow,   { 0xf2, 0xce, 0x65, 0xff } },
        { glacier::ColorCode::Green,    { 0xa0, 0xf2, 0x65, 0xff } },
        { glacier::ColorCode::Teal,     { 0x65, 0xf2, 0xb5, 0xff } },
        { glacier::ColorCode::Blue,     { 0x65, 0xc6, 0xf2, 0xff } },
        { glacier::ColorCode::Lavender, { 0x65, 0x98, 0xf2, 0xff } },

        { glacier::ColorCode::Text,     { 0xc7, 0xd5, 0xed, 0xff } },

        { glacier::ColorCode::Subtext1, { 0xbe, 0xc2, 0xc8, 0xff } },
        { glacier::ColorCode::Subtext0, { 0x90, 0x94, 0x9a, 0xff } },

        { glacier::ColorCode::Overlay2, { 0x6e, 0x71, 0x77, 0xff } },
        { glacier::ColorCode::Overlay1, { 0x5e, 0x61, 0x67, 0xff } },
        { glacier::ColorCode::Overlay0, { 0x52, 0x55, 0x5a, 0xff } },

        { glacier::ColorCode::Surface2, { 0x3d, 0x3f, 0x45, 0xff } },
        { glacier::ColorCode::Surface1, { 0x30, 0x32, 0x37, 0xff } },
        { glacier::ColorCode::Surface0, { 0x24, 0x25, 0x29, 0xff } },

        { glacier::ColorCode::Base1,    { 0x20, 0x22, 0x2a, 0xff } },
        { glacier::ColorCode::Base0,    { 0x19, 0x1a, 0x22, 0xff} }
    };
}
