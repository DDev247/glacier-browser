
#include "../include/colors.h"

glacier::ColorPalette glacier::Colors::m_currentPalette = glacier::ddevDarkPalette;

glm::vec3 glacier::Colors::getColor(glacier::ColorCode code) {
    if (m_currentPalette.contains(code)) {
        return m_currentPalette.at(code);
    }

    return { 0xff, 0x00, 0xff };
}

glm::vec3 glacier::Colors::getColorGL(glacier::ColorCode code) {
    return getColor(code) / 255.0f;
}

/*glacier::ColorPalette glacier::ddevDarkPalette = {
    { glacier::ColorCode::Pink, { 0xd3, 0x99, 0xc7 } },
    { glacier::ColorCode::Red, { 0xe9, 0x51, 0x5b } },
    { glacier::ColorCode::Maroon, { 0xdf, 0x6d, 0x74 } },
    { glacier::ColorCode::Peach, { 0xf4, 0x84, 0x4d } },
    { glacier::ColorCode::Yellow, { 0xf2, 0xce, 0x65 } },
    { glacier::ColorCode::Green, { 0xa0, 0xf2, 0x65 } },
    { glacier::ColorCode::Teal, { 0x65, 0xf2, 0xb5 } },
    { glacier::ColorCode::Blue, { 0x65, 0xc6, 0xf2 } },
    { glacier::ColorCode::Lavender, { 0x65, 0x98, 0xf2 } },

    { glacier::ColorCode::Text, { 0xc7, 0xd5, 0xed } },

    { glacier::ColorCode::Subtext1, { 0xbe, 0xc2, 0xc8 } },
    { glacier::ColorCode::Subtext0, { 0x90, 0x94, 0x9a } },

    { glacier::ColorCode::Overlay2, { 0x6e, 0x71, 0x77 } },
    { glacier::ColorCode::Overlay1, { 0x5e, 0x61, 0x67 } },
    { glacier::ColorCode::Overlay0, { 0x52, 0x55, 0x5a } },

    { glacier::ColorCode::Surface2, { 0x3d, 0x3f, 0x45 } },
    { glacier::ColorCode::Surface1, { 0x30, 0x32, 0x37 } },
    { glacier::ColorCode::Surface0, { 0x24, 0x25, 0x29 } },

    { glacier::ColorCode::Base1, { 0x20, 0x22, 0x2a } },
    { glacier::ColorCode::Base0, { 0x19, 0x1a, 0x22 } }
}; */
