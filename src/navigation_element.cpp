
#include "../include/navigation_element.h"
#include "../include/application.h"
#include "../include/colors.h"
#include "../include/logger.h"

void glacier::NavigationElement::init(Application* app) {
    this->DisplayElement::init(app);
    glm::vec2 size = m_app->getWindowSize();

    m_backButton.init(app, { 5, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/back.png", false);
    m_fwdButton.init(app, { 55, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/fwd.png", false);
    m_refreshButton.init(app, { 55, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/refresh.png", false);

    m_addressBar.init(app, "nav_addressBar", { 105, 45 }, { size.x - 10 - 100 - 150, 40 }, 20, { 0, 0 }, Colors::getColorGL(ColorCode::Text), Colors::getColorGL(ColorCode::Subtext1), Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), false);
    m_addressBar.m_placeholder = "Search using: ";

    m_homeButton.init(app, { size.x - 135, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/home.png", false);
    m_downloadsButton.init(app, { size.x - 90, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/download.png", false);
    m_menuButton.init(app, { size.x - 45, 45 }, 20, Colors::getColorGL(ColorCode::Surface2), Colors::getColorGL(ColorCode::Overlay0), "textures/menu.png", false);
}

void glacier::NavigationElement::render() {
    float dt = m_app->getDeltaSeconds();
    m_drawFwd = m_history.size() > 0;

    // Background
    m_app->drawRectFill({ 0, 40 }, { m_app->getWindowSize().x, 50 }, Colors::getColorGL(ColorCode::Surface1));

    // buttons
    m_backButton.render(dt);

    glm::vec2 size = m_app->getWindowSize();
    if(m_drawFwd) {
        m_fwdButton.render(dt);
        m_refreshButton.m_position = { 105, 45 };

        m_addressBar.m_position = { 155, 45 };
        m_addressBar.m_size = { size.x - 5 - 155 - 135, 40 };
    }
    else {
        m_refreshButton.m_position = { 55, 45 };
        
        m_addressBar.m_position = { 105, 45 };
        m_addressBar.m_size = { size.x - 5 - 105 - 135, 40 };
    }

    m_homeButton.m_position = { size.x - 135, 45 };
    m_downloadsButton.m_position = { size.x - 90, 45 };
    m_menuButton.m_position = { size.x - 45, 45 };

    m_refreshButton.render(dt);

    m_addressBar.render(dt);

    m_homeButton.render(dt);
    m_downloadsButton.render(dt);
    m_menuButton.render(dt);
}
