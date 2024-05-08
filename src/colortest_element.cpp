
#include "../include/navigation_element.h"
#include "../include/application.h"
#include "../include/colors.h"

void glacier::ColorTestElement::render() {
	m_app->drawText("Color test:", m_app->getRoboto(), { 10, 10 }, Colors::getColorGL(ColorCode::Text), 0.5f);

	m_app->drawRectRoundedFill({50, 50}, {100, 100}, Colors::getColorGL(ColorCode::Pink), 25);
	m_app->drawRectRoundedFill({ 175, 50 }, { 100, 100 }, Colors::getColorGL(ColorCode::Red), 25);
	m_app->drawRectRoundedFill({ 300, 50 }, { 100, 100 }, Colors::getColorGL(ColorCode::Maroon), 25);
	m_app->drawRectRoundedFill({ 425, 50 }, { 100, 100 }, Colors::getColorGL(ColorCode::Peach), 25);
	m_app->drawRectRoundedFill({ 550, 50 }, { 100, 100 }, Colors::getColorGL(ColorCode::Yellow), 25);

	m_app->drawRectRoundedFill({ 50, 175 }, { 100, 100 }, Colors::getColorGL(ColorCode::Green), 25);
	m_app->drawRectRoundedFill({ 175, 175 }, { 100, 100 }, Colors::getColorGL(ColorCode::Teal), 25);
	m_app->drawRectRoundedFill({ 300, 175 }, { 100, 100 }, Colors::getColorGL(ColorCode::Blue), 25);
	m_app->drawRectRoundedFill({ 425, 175 }, { 100, 100 }, Colors::getColorGL(ColorCode::Lavender), 25);

	m_app->drawText("Text/Subtext", m_app->getRoboto(), { 425, 300 }, Colors::getColorGL(ColorCode::Text), 0.5f);
	m_app->drawRectRoundedFill({ 50, 300 }, { 100, 100 }, Colors::getColorGL(ColorCode::Text), 25);
	m_app->drawRectRoundedFill({ 175, 300 }, { 100, 100 }, Colors::getColorGL(ColorCode::Subtext1), 25);
	m_app->drawRectRoundedFill({ 300, 300 }, { 100, 100 }, Colors::getColorGL(ColorCode::Subtext0), 25);
	
	m_app->drawText("Overlay", m_app->getRoboto(), { 425, 425 }, Colors::getColorGL(ColorCode::Text), 0.5f);
	m_app->drawRectRoundedFill({ 50, 425 }, { 100, 100 }, Colors::getColorGL(ColorCode::Overlay2), 25);
	m_app->drawRectRoundedFill({ 175, 425 }, { 100, 100 }, Colors::getColorGL(ColorCode::Overlay1), 25);
	m_app->drawRectRoundedFill({ 300, 425 }, { 100, 100 }, Colors::getColorGL(ColorCode::Overlay0), 25);

	m_app->drawText("Surface", m_app->getRoboto(), { 425, 550 }, Colors::getColorGL(ColorCode::Text), 0.5f);
	m_app->drawRectRoundedFill({ 50, 550 }, { 100, 100 }, Colors::getColorGL(ColorCode::Surface2), 25);
	m_app->drawRectRoundedFill({ 175, 550 }, { 100, 100 }, Colors::getColorGL(ColorCode::Surface1), 25);
	m_app->drawRectRoundedFill({ 300, 550 }, { 100, 100 }, Colors::getColorGL(ColorCode::Surface0), 25);
	
	m_app->drawText("Base", m_app->getRoboto(), { 300, 675 }, Colors::getColorGL(ColorCode::Text), 0.5f);
    m_app->drawRectRoundedFill({ 50, 675 }, { 100, 100 }, Colors::getColorGL(ColorCode::Base1), 25);
    m_app->drawRectRoundedFill({ 175, 675 }, { 100, 100 }, Colors::getColorGL(ColorCode::Base0), 25);
}
