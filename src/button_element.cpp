
#include "../include/button_element.h"
#include "../include/application.h"
#include "../include/colors.h"

glm::vec4 glacier::ButtonElement::modColor(glm::vec4 color, float alpha) {
    return { color.r, color.g, color.b, alpha };
}

void glacier::ButtonElement::render(float dt) {
    
    /* ------------------------------- Processing ------------------------------- */
    
    if(m_app->checkCollisionRect(m_app->getCursor(), m_position, { m_radius * 2, m_radius * 2 })) {
        m_alpha += 4.0f * dt;
        if(m_app->getMouseButtons().left) {
            m_colorLerp += 25.0f * dt;
        }
        else {
            m_colorLerp -= 25.0f * dt;
        }
    }
    else {
        m_alpha -= 4.0f * dt;
        m_colorLerp = 0;
    }

    m_alpha = std::clamp(m_alpha, 0.0f, 1.0f);
    m_colorLerp = std::clamp(m_colorLerp, 0.0f, 1.0f);

    /* --------------------------------- Drawing -------------------------------- */
    
    // Bounding box
    if(m_boundingBox)
        m_app->drawRect(m_position, { m_radius * 2, m_radius * 2 }, Colors::getColorGL(ColorCode::Red));

    float alphaPrecomp = m_alpha * m_alpha;

    glm::vec4 hoverColor = modColor(m_hoverColor, alphaPrecomp);
    glm::vec4 holdColor = modColor(m_holdColor, alphaPrecomp);
    
    glm::vec4 color = glm::mix(hoverColor, holdColor, m_colorLerp);
    
    m_app->drawCircleFill(m_position, color, m_radius);
    m_app->drawTexture(m_texture, m_position + glm::vec2{ 5, 5 }, { m_radius * 2 - 10, m_radius * 2 - 10 }, color);
}
