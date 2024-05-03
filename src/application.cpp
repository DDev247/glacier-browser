
#include "../include/application.h"
#include "../include/logger.h"
#include "../include/colors.h"

#include <iostream>
#include <exception>
#define _USE_MATH_DEFINES
#include <math.h>

#define CREATE_CALLBACK_1(name, log, arg1type, arg1) [](arg1type arg1) { \
	glacier::Application* app = static_cast<glacier::Application*>(glfwGetWindowUserPointer(window)); \
	if(log) LOG(Log::Level::Debug, "glacier::Application::initWindow", #name); \
	if (app) app->name(arg1); \
	}

#define CREATE_CALLBACK_2(name, log, arg1type, arg1, arg2type, arg2) [](arg1type arg1m, arg2type arg2) { \
	glacier::Application* app = static_cast<glacier::Application*>(glfwGetWindowUserPointer(window)); \
	if(log) LOG(Log::Level::Debug, "glacier::Application::initWindow", #name); \
	if (app) app->name(arg1, arg2); \
	}

#define CREATE_CALLBACK_3(name, log, arg1type, arg1, arg2type, arg2, arg3type, arg3) [](arg1type arg1, arg2type arg2, arg3type arg3) { \
	glacier::Application* app = static_cast<glacier::Application*>(glfwGetWindowUserPointer(window)); \
	if(log) LOG(Log::Level::Debug, "glacier::Application::initWindow", #name); \
	if (app) app->name(arg1, arg2, arg3); \
	}

void glacier::Application::initWindow()
{
	LOG(Log::Level::Debug, "glacier::Application::initWindow", "Initialising GLFW");
	if (!glfwInit()) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialise GLFW.");
	}

	/* Create a windowed mode window and its OpenGL context */
	LOG(Log::Level::Debug, "glacier::Application::initWindow", "Creating main window");
	m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, "Glacier", NULL, NULL);
	if (!m_window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create window.");
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	// Assign events
	glfwSetWindowUserPointer(m_window, this); // Pass the 'this' pointer to GLFW callbacks
	glfwSetWindowRefreshCallback(m_window, CREATE_CALLBACK_1(windowRefreshCallback, false, GLFWwindow*, window));
	glfwSetFramebufferSizeCallback(m_window, CREATE_CALLBACK_3(framebufferSizeCallback, false, GLFWwindow*, window, int, width, int, height));
	glfwSetCursorPosCallback(m_window, CREATE_CALLBACK_3(cursorPosCallback, false, GLFWwindow*, window, double, xpos, double, ypos));
}

void glacier::Application::windowRefreshCallback(GLFWwindow* window) {
	render();
	glfwSwapBuffers(m_window);
	glFinish();
}

void glacier::Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	m_windowSize.x = width;
	m_windowSize.y = height;
	glViewport(0, 0, m_windowSize.x, m_windowSize.y);
}

void glacier::Application::cursorPosCallback(GLFWwindow * window, double xpos, double ypos) {
	m_cursor.x = xpos;
	m_cursor.y = ypos;
}

void glacier::Application::mainLoop() {
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		render();

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	LOG(Log::Level::Debug, "glacier::Application::mainLoop", "Window closing");
}

void glacier::Application::drawLine(glm::vec2 from, glm::vec2 to, float width) {
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2f(from.x, from.y);
	glVertex2f(to.x, to.y);
	glEnd();
}

void glacier::Application::drawCircle(glm::vec2 position, float radius, int points) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((360 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + radius - x, position.y + radius - y);
	}
	glVertex2f(position.x, position.y + radius);
	glEnd();
}

void glacier::Application::drawCircleFill(glm::vec2 position, glm::vec3 color, float radius, int points) {
	setColorGL(color);
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x + radius, position.y + radius);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((360 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + radius - x, position.y + radius - y);
	}
	glVertex2f(position.x, position.y + radius);
	glEnd();
}

void glacier::Application::drawRect(glm::vec2 position, glm::vec2 size) {
	glBegin(GL_LINES);
	glVertex2f(position.x, position.y);
	glVertex2f(position.x + size.x, position.y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(position.x + size.x, position.y);
	glVertex2f(position.x + size.x, position.y + size.y);
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(position.x + size.x, position.y + size.y);
	glVertex2f(position.x, position.y + size.y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(position.x, position.y + size.y);
	glVertex2f(position.x, position.y);
	glEnd();
}

void glacier::Application::drawRectFill(glm::vec2 position, glm::vec2 size, glm::vec3 color) {
	setColorGL(color);

	glBegin(GL_TRIANGLES);
	glVertex2f(position.x, position.y);
	glVertex2f(position.x + size.x, position.y);
	glVertex2f(position.x + size.x, position.y + size.y);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(position.x, position.y);
	glVertex2f(position.x, position.y + size.y);
	glVertex2f(position.x + size.x, position.y + size.y);
	glEnd();
}

void glacier::Application::drawRectRounded(glm::vec2 position, glm::vec2 size, float radius, int points) {

	// top left
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + radius - x, position.y + radius - y);
	}
	glVertex2f(position.x + radius, position.y);
	glEnd();

	// top right
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + size.x - radius + x, position.y + radius - y);
	}
	glVertex2f(position.x + size.x - radius, position.y);
	glEnd();

	// bottom left
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + radius - x, position.y + size.y - radius + y);
	}
	glVertex2f(position.x + radius, position.y + size.y);
	glEnd();

	// bottom right
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + size.x - radius + x, position.y + size.y - radius + y);
	}
	glVertex2f(position.x + size.x - radius, position.y + size.y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(position.x + radius, position.y);
	glVertex2f(position.x + size.x - radius, position.y);

	glVertex2f(position.x + size.x, position.y + radius);
	glVertex2f(position.x + size.x, position.y + size.y - radius);
	
	glVertex2f(position.x + size.x - radius, position.y + size.y);
	glVertex2f(position.x + radius, position.y + size.y);
	
	glVertex2f(position.x, position.y + size.y - radius);
	glVertex2f(position.x, position.y + radius);
	glEnd();
}

void glacier::Application::drawRectRoundedFill(glm::vec2 position, glm::vec2 size, glm::vec3 color, float radius, int points) {
	setColorGL(color);

	// top left
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x + radius, position.y + radius);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + radius - x, position.y + radius - y);
	}
	glVertex2f(position.x + radius, position.y);
	//glVertex2f(position.x + radius, position.y + radius);
	glEnd();

	// top right
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x + size.x - radius, position.y + radius);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + size.x - radius + x, position.y + radius - y);
	}
	glVertex2f(position.x + size.x - radius, position.y);
	//glVertex2f(position.x + size.x + radius, position.y + radius);
	glEnd();

	// bottom left
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x + radius, position.y + size.y - radius);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + radius - x, position.y + size.y - radius + y);
	}
	glVertex2f(position.x + radius, position.y + size.y);
	//glVertex2f(position.x + size.x + radius, position.y + radius);
	glEnd();

	// bottom right
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x + size.x - radius, position.y + size.y - radius);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		glVertex2f(position.x + size.x - radius + x, position.y + size.y - radius + y);
	}
	glVertex2f(position.x + size.x - radius, position.y + size.y);
	//glVertex2f(position.x + size.x + radius, position.y + radius);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(position.x + radius, position.y);
	glVertex2f(position.x + size.x - radius, position.y);
	glVertex2f(position.x + size.x - radius, position.y + radius);

	glVertex2f(position.x + size.x - radius, position.y + radius);
	glVertex2f(position.x + radius, position.y + radius);
	glVertex2f(position.x + radius, position.y);

	glVertex2f(position.x + radius, position.y + size.y);
	glVertex2f(position.x + size.x - radius, position.y + size.y);
	glVertex2f(position.x + size.x - radius, position.y - radius + size.y);

	glVertex2f(position.x + size.x - radius, position.y - radius + size.y);
	glVertex2f(position.x + radius, position.y - radius + size.y);
	glVertex2f(position.x + radius, position.y + size.y);

	glVertex2f(position.x, position.y + radius);
	glVertex2f(position.x + size.x, position.y - radius + size.y);
	glVertex2f(position.x, position.y - radius + size.y);

	glVertex2f(position.x, position.y + radius);
	glVertex2f(position.x + size.x, position.y + radius);
	glVertex2f(position.x + size.x, position.y - radius + size.y);
	glEnd();
}

void glacier::Application::render() {
	glm::vec3 cc = Colors::getColorGL(ColorCode::Base0);
	glClearColor(cc.r, cc.g, cc.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, m_windowSize.x, m_windowSize.y, 0.0f, 0.0f, 1.0f);
	//glOrtho(0.0, m_windowSize.x, m_windowSize.y, 0.0, -1, 1);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, 0.0);
	
	glLineWidth(1.0);
	setColor(Colors::getColor(ColorCode::Lavender));
	drawRectRounded({50, 50}, {100, 100}, 5, 16);
	drawRectRounded({ 175, 50 }, { 100, 100 }, 10, 16);
	drawRectRounded({ 300, 50 }, { 100, 100 }, 15, 16);
	drawRectRounded({ 425, 50 }, { 100, 100 }, 20, 16);
	glLineWidth(2.0);
	drawRectRounded({ 550, 50 }, { 100, 100 }, 25, 16);
	drawRectRoundedFill({ 550, 50 }, { 100, 100 }, Colors::getColorGL(ColorCode::Maroon), 25, 16);
	setColor(Colors::getColor(ColorCode::Lavender));
	glLineWidth(1.0);

	drawRectRounded({ 50, 175 }, { 100, 100 }, 25, 32);
	drawRectRounded({ 175, 175 }, { 100, 100 }, 25, 16);
	drawRectRounded({ 300, 175 }, { 100, 100 }, 25, 8);
	drawRectRounded({ 425, 175 }, { 100, 100 }, 25, 4);
	drawRectRounded({ 550, 175 }, { 100, 100 }, 25, 2);

	drawCircle({ 50, 300 }, 10, 16);
	drawCircle({ 175, 300 }, 20, 16);
	drawCircle({ 300, 300 }, 30, 16);
	drawCircle({ 425, 300 }, 40, 16);
	drawCircleFill({ 550, 300 }, Colors::getColorGL(ColorCode::Maroon), 50, 16);

	setColorGL(Colors::getColorGL(ColorCode::Green));
	drawRect({ 50, 425 }, { 100, 100 }); glLineWidth(2.0f);
	setColorGL(Colors::getColorGL(ColorCode::Pink));
	drawRect({ 175, 425 }, { 100, 100 });
	drawRectFill({ 300, 425 }, { 100, 100 }, Colors::getColorGL(ColorCode::Peach));
	drawRectFill({ 425, 425 }, { 100, 100 }, Colors::getColorGL(ColorCode::Yellow));
	setColorGL(Colors::getColorGL(ColorCode::Red));
	drawLine({ 550, 425 }, { 650, 525 }, 2);

	//drawRectRounded({ 50, 50 }, { 400, 400 }, 100, 16);

	// cursor
	drawRectFill({ m_cursor.x - 5, m_cursor.y - 5 }, { 10, 10 }, Colors::getColorGL(ColorCode::Overlay0));
	
	glPopMatrix();
}

void glacier::Application::cleanup() {
	LOG(Log::Level::Debug, "glacier::Application::cleanup", "Terminating GLFW");
	glfwTerminate();
}
