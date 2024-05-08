
#pragma once

#include "display_element.h"
#include <glm/glm.hpp>
#include <string>

namespace glacier {
    
	class InputElement : public DisplayElement {
	public:
		void init(Application* app, std::string id, glm::vec2 position, glm::vec2 size, float radius, glm::vec2 padding, glm::vec4 textColor, glm::vec4 placeholderColor, glm::vec4 color, glm::vec4 holdColor, bool boundingBox = false) {
            DisplayElement::init(app);
            m_id = id;
            m_position = position;
            m_size = size;
            m_radius = radius;
            m_padding = padding;
            m_textColor = textColor;
            m_placeholderColor = placeholderColor;
            m_color = color;
            m_holdColor = holdColor;
            m_boundingBox = boundingBox;
        };
        void render(float dt);

        glm::vec2 m_position;
        glm::vec2 m_size;
        int m_textSize = 20;
        
        std::string m_placeholder;
        std::string m_text;

    private:
		float m_colorLerp = 0.0f;

        float m_radius;
        glm::vec2 m_padding;
        glm::vec4 m_color, m_holdColor;
        glm::vec4 m_textColor, m_placeholderColor;
        bool m_boundingBox;

        std::string m_id;
        bool m_focused;
        void keyCallback(const char character);
	};

}
