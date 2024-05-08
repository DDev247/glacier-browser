
#include "../include/navigation_element.h"
#include "../include/application.h"
#include "../include/colors.h"
#include "../include/logger.h"

void glacier::NavigationElement::init(Application* app) {
    this->DisplayElement::init(app);
    m_backButton.init(app, { 5, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/back.png", false);
    m_fwdButton.init(app, { 55, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/fwd.png", false);
    m_refreshButton.init(app, { 55, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/refresh.png", false);
    m_addressBar.init(app, "nav_addressBar", { 105, 45 }, { 400, 40 }, 20, { 0, 0 }, Colors::getColorGL(ColorCode::Text), Colors::getColorGL(ColorCode::Subtext1), Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), false);
    m_addressBar.m_placeholder = "Search using: ";
}

void glacier::NavigationElement::render() {
    float dt = m_app->getDeltaSeconds();
    m_drawFwd = m_history.size() > 0;

    // Background
    m_app->drawRectFill({ 0, 40 }, { m_app->getWindowSize().x, 50 }, Colors::getColorGL(ColorCode::Surface1));

    // buttons
    m_backButton.render(dt);

    if(m_drawFwd) {
        m_fwdButton.render(dt);
        m_refreshButton.m_position = { 105, 45 };

        m_addressBar.m_position = { 155, 45 };
        m_addressBar.m_size = { 350, 45 };
    }
    else {
        m_refreshButton.m_position = { 55, 45 };
        
        m_addressBar.m_position = { 105, 45 };
        m_addressBar.m_size = { 400, 40 };
    }

    m_refreshButton.render(dt);

    m_addressBar.render(dt);
}
