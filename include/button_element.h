
#pragma once

#include "display_element.h"
#include "texture.h"
#include <glm/glm.hpp>

namespace glacier {
    
	class ButtonElement : public DisplayElement {
	public:
		void init(Application* app, glm::vec2 position, float radius, glm::vec4 hoverColor, glm::vec4 holdColor, std::string texturePath, bool boundingBox = false) {
            DisplayElement::init(app);
            m_position = position;
            m_radius = radius;
            m_hoverColor = hoverColor;
            m_holdColor = holdColor;
            m_boundingBox = boundingBox;
            m_texture.load(texturePath);
        };
        void render(float dt);

        glm::vec2 m_position;

    private:
        glm::vec4 modColor(glm::vec4 color, float alpha);

		float m_alpha = 0.0f;
        float m_colorLerp = 0.0f;

        float m_radius;
        glm::vec4 m_hoverColor, m_holdColor;
        Texture m_texture;
        bool m_boundingBox;
	};

}
