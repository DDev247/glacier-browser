
#include "../include/input_element.h"
#include "../include/application.h"
#include "../include/logger.h"
#include "../include/colors.h"

void glacier::InputElement::keyCallback(const char character) {
    m_text += character;
}

void glacier::InputElement::render(float dt) {
    
    /* ------------------------------- Processing ------------------------------- */
    
    if(m_app->checkCollisionRect(m_app->getCursor(), m_position, m_size)) {
        if(m_app->getMouseButtons().left) {
            m_colorLerp += 25.0f * dt;
        }
        else {
            m_colorLerp -= 25.0f * dt;
        }

        // Check for first frame press
        if(m_app->getMouseButtonsFirstFrame().left) {
            m_focused = true;
            m_app->setKeyCharSubscriber([](const char character, void* userParam) {
                InputElement* ie = static_cast<InputElement*>(userParam);
                ie->keyCallback(character);
            }, (void*)this, m_id);
        }
    }
    else {
        m_colorLerp = 0;
    }

    m_colorLerp = std::clamp(m_colorLerp, 0.0f, 1.0f);

    /* --------------------------------- Drawing -------------------------------- */
    
    // Bounding box
    if(m_boundingBox)
        m_app->drawRect(m_position, m_size, Colors::getColorGL(ColorCode::Red));

    glm::vec4 color = glm::mix(m_color, m_holdColor, m_colorLerp + m_focused);
    
    m_app->drawRectRoundedFill(m_position, m_size, color, m_radius);

    float y = (m_size.y - m_textSize * 1.5) / 2;
    if(m_text.length() == 0)
        m_app->drawText(m_placeholder, m_app->getRoboto(), m_position + m_padding + glm::vec2{ m_radius / 2, y }, m_placeholderColor, m_textSize);
    else
        m_app->drawText(m_text, m_app->getRoboto(), m_position + m_padding + glm::vec2{ m_textSize / 2, m_textSize / 2 }, m_textColor, m_textSize);
}
