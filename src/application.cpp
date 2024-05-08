
#include "../include/application.h"
#include "../include/logger.h"
#include "../include/colors.h"

#include <iostream>
#include <exception>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>

/* -------------------------- GLFW callback macros -------------------------- */

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

#define CREATE_CALLBACK_4(name, log, arg1type, arg1, arg2type, arg2, arg3type, arg3, arg4type, arg4) [](arg1type arg1, arg2type arg2, arg3type arg3, arg4type arg4) { \
	glacier::Application* app = static_cast<glacier::Application*>(glfwGetWindowUserPointer(window)); \
	if(log) LOG(Log::Level::Debug, "glacier::Application::initWindow", #name); \
	if (app) app->name(arg1, arg2, arg3, arg4); \
	}

// Big buffer size
#define VERTEX_BUFFER_SIZE 2 * 1000

// Initializes the main window
void glacier::Application::initWindow() {

	/* ----------------------------- Initialize GLFW ---------------------------- */
	
	LOG(Log::Level::Debug, "glacier::Application::initWindow", "Initialising GLFW");
	if (!glfwInit()) {
		const char* description;
		int code = glfwGetError(&description);
		std::string descStr;
		if (description) descStr = std::string(description);
		else descStr = "None";
		
		glfwTerminate();
		throw std::runtime_error("Failed to initialise GLFW. GLFW Error: " + descStr);
	}

	/* ------------------------------ Create window ----------------------------- */
	
	LOG(Log::Level::Debug, "glacier::Application::initWindow", "Creating main window");
	m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, "Glacier", NULL, NULL);
	if (!m_window) {
		const char* description;
		int code = glfwGetError(&description);
		std::string descStr;
		if (description) descStr = std::string(description);
		else descStr = "None";

		glfwTerminate();
		throw std::runtime_error("Failed to create window. GLFW Error: " + descStr);
	}

	/* -------------------- Make the window's context current ------------------- */
	
	glfwMakeContextCurrent(m_window);

	/* ------------------------------- Load OpenGL ------------------------------ */

	LOG(Log::Level::Debug, "glacier::Application::initWindow", "Loading OpenGL");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		throw std::runtime_error("Failed to load OpenGL.");
	}

	/* --------------------------- Get OpenGL version --------------------------- */
	
	std::string glVersion(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	std::string glslVersion(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	std::string glVendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));

	LOG(Log::Level::Debug, "glacier::Application::initWindow", "OpenGL version " + glVersion + ", glsl " + glslVersion + " from " + glVendor);

	/* ------------------------------ Assign events ----------------------------- */
	glfwSetWindowUserPointer(m_window, this); // Pass the 'this' pointer to GLFW callbacks
	glfwSetWindowRefreshCallback(m_window, CREATE_CALLBACK_1(windowRefreshCallback, false, GLFWwindow*, window));
	glfwSetFramebufferSizeCallback(m_window, CREATE_CALLBACK_3(framebufferSizeCallback, false, GLFWwindow*, window, int, width, int, height));
	glfwSetCursorPosCallback(m_window, CREATE_CALLBACK_3(cursorPosCallback, false, GLFWwindow*, window, double, xpos, double, ypos));
	glfwSetMouseButtonCallback(m_window, CREATE_CALLBACK_4(mouseButtonCallback, false, GLFWwindow*, window, int, button, int, action, int, mods));


    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		const glacier::Application* app = static_cast<const glacier::Application*>(userParam);
		app->debugMessageCallback(source, type, id, severity, length, message, userParam);
	}, (void*)this);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	/* ------------------------------ Enable VSync ------------------------------ */
	
	glfwSwapInterval(1);

	/* ----------------------------- Enable blending ---------------------------- */
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* --------------------------- Initialise elements -------------------------- */
	
	m_colorTest.init(this);
	m_navigation.init(this);
}

void glacier::Application::windowRefreshCallback(GLFWwindow* window) {
	// Do whole render cycle (to avoid black bar while resizing)
	render();
	glfwSwapBuffers(m_window);
	glFinish();
}

void glacier::Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// Set window size vector and resize FB
	m_windowSize.x = width;
	m_windowSize.y = height;
	glViewport(0, 0, m_windowSize.x, m_windowSize.y);
}

void glacier::Application::cursorPosCallback(GLFWwindow * window, double xpos, double ypos) {
	m_cursor.x = xpos;
	m_cursor.y = ypos;
}

void glacier::Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	bool value;
	if(action == GLFW_PRESS) value = true;
	else if(action == GLFW_RELEASE) value = false;

	switch(button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			m_mouse.left = value;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			m_mouse.middle = value;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			m_mouse.right = value;
			break;
	}
}

void glacier::Application::debugMessageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam) const {
	std::string sourceStr;
	std::string typeStr;
	std::string severityStr;

	switch(source) {
		case GL_DEBUG_SOURCE_API:				sourceStr = "API"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		sourceStr = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER:				sourceStr = "Other"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	sourceStr = "Shader compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		sourceStr = "Third party"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		sourceStr = "Window System"; break;
	}

	switch(type) {
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	typeStr = "Deprecated behavior"; break;
		case GL_DEBUG_TYPE_ERROR:			 	typeStr = "Error"; break;
		case GL_DEBUG_TYPE_MARKER:			 	typeStr = "Marker"; break;
		case GL_DEBUG_TYPE_OTHER:			 	typeStr = "Other"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:			typeStr = "Performance"; break;
		case GL_DEBUG_TYPE_POP_GROUP:			typeStr = "Pop group"; break;
		case GL_DEBUG_TYPE_PORTABILITY:			typeStr = "Portability"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:			typeStr = "Push group"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	typeStr = "Undefined behavior"; break;
	}

	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:			severityStr = "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:			severityStr = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW:				severityStr = "LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	severityStr = "NOTIF"; break;
	}

	LOG(Log::Level::Debug, "glacier::Application::debugMessageCallback", severityStr + " - " + sourceStr + " says " + std::string(message) + " because of " + typeStr);
}

void glacier::Application::loadFonts() {
	// Load roboto font
	bool robotoFailed;
	m_roboto = Text::loadFont("fonts/roboto.ttf", &robotoFailed, 1024);
	if(robotoFailed) {
		throw std::runtime_error("Failed to load Roboto font.");
	}
}

void glacier::Application::loadShaders() {
	// Load text shader
	bool textShaderCompiled = m_textShader.compile("shaders/text.vert", "shaders/text.frag");
	if(!textShaderCompiled) {
		throw std::runtime_error("Text shader failed to compile.");
	}

	// Load regular shader
	bool regularShaderCompiled = m_regularShader.compile("shaders/regular.vert", "shaders/regular.frag");
	if(!regularShaderCompiled) {
		throw std::runtime_error("Regular shader failed to compile");
	}

	// Load texture shader
	bool textureShaderCompiled = m_textureShader.compile("shaders/texture.vert", "shaders/texture.frag");
	if(!textureShaderCompiled) {
		throw std::runtime_error("Texture shader failed to compile");
	}
}

void glacier::Application::setupBuffers() {

	/* ------------------------------- Text buffer ------------------------------ */
	
	// Create Buffers
	glGenVertexArrays(1, &m_textVAO);
	glGenBuffers(1, &m_textVBO);

	// Bind buffers
	glBindVertexArray(m_textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
	
	// Allocate VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	// Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* ------------------------------- Line buffer ------------------------------ */

	// Create Buffers
	glGenVertexArrays(1, &m_lineVAO);
	glGenBuffers(1, &m_lineVBO);

	// Bind buffers
	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	
	// Allocate VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, NULL, GL_DYNAMIC_DRAW);

	// Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* ------------------------------- Big buffer ------------------------------- */

	// Create Buffers
	glGenVertexArrays(1, &m_bigVAO);
	glGenBuffers(1, &m_bigVBO);
	
	// Bind buffers
	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);
	
	// Allocate VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTEX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	// Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* ----------------------------- Texture buffer ----------------------------- */

	// Create Buffers
	glGenVertexArrays(1, &m_textureVAO);
	glGenBuffers(1, &m_textureVBO);
	
	// Bind buffers
	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	
	// Allocate VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, NULL, GL_DYNAMIC_DRAW);

	// Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void glacier::Application::mainLoop() {
	while (!glfwWindowShouldClose(m_window)) {
		// Render to the frame buffer
		render();

		// Swap buffers and poll events
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	LOG(Log::Level::Debug, "glacier::Application::mainLoop", "Window closing");
}

void glacier::Application::drawLine(glm::vec2 from, glm::vec2 to, glm::vec4 color, float width) {
	m_regularShader.use();
	// Setup shader
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);
	glLineWidth(width);

	// Bind line buffer
	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);

	float vertices[2][2] = {
		{ from.x, from.y, },
		{ to.x, to.y, },
	};
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void glacier::Application::drawCircle(glm::vec2 position, float radius, glm::vec4 color, float width, int points) {
	const int size = (points * 2) + 2;
	//std::list<float> vertices;
	float* vertices = new float[size];
	//glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((360 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		//vertices.push_back(position.x + radius - x);
		//vertices.push_back(position.y + radius - y);
		assert(i * 2 < size && i * 2 + 1 < size);
		vertices[i * 2] = position.x + radius - x;
		vertices[i * 2 + 1] = position.y + radius - y;
	}
	//vertices.push_back(position.x);
	//vertices.push_back(position.y + radius);
	vertices[size - 2] = position.x;
	vertices[size - 1] = position.y + radius;
	//glVertex2f(position.x, position.y + radius);
	//glEnd();

	m_regularShader.use();
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);
	glLineWidth(width);

	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_LINE_STRIP, 0, size / 2);
	glBindVertexArray(0);

	//free(vertices);
	delete[] vertices;
}

void glacier::Application::drawCircleFill(glm::vec2 position, glm::vec4 color, float radius, int points) {
	m_regularShader.use();

	// Setup shader
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);

	// Bind big buffer
	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	// Create local vertex array
	const int size = (points + 2) * 2;
	const int size2 = points + 2;
	float* vertices = new float[size];

	// Vertex 0
	vertices[0] = position.x + radius; vertices[1] = position.y + radius;
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((360 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices[(i * 2) + 2] = position.x + radius - x;
		vertices[(i * 2) + 3] = position.y + radius - y;
	}
	vertices[size - 2] = position.x;
	vertices[size - 1] = position.y + radius;

	// Push vertex array to VBO and unbind it
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Draw triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, size2);

	// Unbind VAO and delete local vertex array
	glBindVertexArray(0);
	delete[] vertices;
}

void glacier::Application::drawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color, float width) {
	m_regularShader.use();
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);
	glLineWidth(width);

	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	float vertices[16] = {
		position.x, position.y,
		position.x + size.x, position.y,
		position.x + size.x, position.y,
		position.x + size.x, position.y + size.y,
		position.x + size.x, position.y + size.y,
		position.x, position.y + size.y,
		position.x, position.y + size.y,
		position.x, position.y
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_LINES, 0, 8);
	glBindVertexArray(0);
}

void glacier::Application::drawRectFill(glm::vec2 position, glm::vec2 size, glm::vec4 color) {
	m_regularShader.use();
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);

	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	float vertices[12] = {
		position.x, position.y,
		position.x, position.y + size.y,
		position.x + size.x, position.y + size.y,
		position.x, position.y,
		position.x + size.x, position.y + size.y,
		position.x + size.x, position.y,
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 12, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void glacier::Application::drawRectRounded(glm::vec2 position, glm::vec2 size, glm::vec4 color, float radius, int points) {
	m_regularShader.use();

	// Setup shader
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);

	// Bind big buffer
	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	// Create local vertex array
	const int vsize = points * 8 + 10;
	const int vsize2 = points * 4 + 5;
	float* vertices = new float[vsize];

	// top left
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices[i * 2] = position.x + radius - x;
		vertices[i * 2 + 1] = position.y + radius - y;
	}
	vertices[points * 2] = position.x + radius;
	vertices[points * 2 + 1] = position.y;

	// top right
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = sinf(anglerad) * radius;
		float y = cosf(anglerad) * radius;
		vertices[points * 2 + 2 + i * 2] = position.x + size.x - radius + x;
		vertices[points * 2 + 2 + i * 2 + 1] = position.y + radius - y;
	}
	vertices[points * 4 + 2] = position.x + size.x;
	vertices[points * 4 + 2 + 1] = position.y + radius;

	// bottom right
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices[points * 4 + 4 + i * 2] = position.x + size.x - radius + x;
		vertices[points * 4 + 4 + i * 2 + 1] = position.y + size.y - radius + y;
	}
	vertices[points * 6 + 4] = position.x + size.x - radius;
	vertices[points * 6 + 4 + 1] = position.y + size.y;

	// bottom left
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = sinf(anglerad) * radius;
		float y = cosf(anglerad) * radius;
		vertices[points * 6 + 6 + i * 2] = position.x + radius - x;
		vertices[points * 6 + 6 + i * 2 + 1] = position.y + size.y - radius + y;
	}
	vertices[points * 8 + 6] = position.x;
	vertices[points * 8 + 6 + 1] = position.y + size.y - radius;
	
	vertices[points * 8 + 8] = position.x;
	vertices[points * 8 + 8 + 1] = position.y + radius;

	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vsize, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_LINE_STRIP, 0, vsize2);
	glBindVertexArray(0);

	delete[] vertices;
}

void glacier::Application::drawRectRoundedFill(glm::vec2 position, glm::vec2 size, glm::vec4 color, float radius, int points) {
	m_regularShader.use();

	// Setup shader
	glUniformMatrix4fv(glGetUniformLocation(m_regularShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_regularShader.programID, "color"), color.r, color.g, color.b, color.a);

	// Bind big buffer
	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	// Create local vertex array
	const int vsize = points * 2 + 4;
	const int vsize2 = points + 2;
	float* vertices1 = new float[vsize];
	float* vertices2 = new float[vsize];
	float* vertices3 = new float[vsize];
	float* vertices4 = new float[vsize];

	// top left
	vertices1[0] = position.x + radius;
	vertices1[1] = position.y + radius;
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices1[2 + i * 2] = position.x + radius - x;
		vertices1[2 + i * 2 + 1] = position.y + radius - y;
	}
	vertices1[2 + points * 2] = position.x + radius;
	vertices1[2 + points * 2 + 1] = position.y;

	// top right
	vertices2[0] = position.x + size.x - radius;
	vertices2[1] = position.y + radius;
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices2[2 + i * 2] = position.x + size.x - radius + x;
		vertices2[2 + i * 2 + 1] = position.y + radius - y;
	}
	vertices2[2 + points * 2] = position.x + size.x - radius;
	vertices2[2 + points * 2 + 1] = position.y;

	// bottom left
	vertices3[0] = position.x + radius;
	vertices3[1] = position.y + size.y - radius;
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices3[2 + i * 2] = position.x + radius - x;
		vertices3[2 + i * 2 + 1] = position.y + size.y - radius + y;
	}
	vertices3[2 + points * 2] = position.x + radius;
	vertices3[2 + points * 2 + 1] = position.y + size.y;

	// bottom right
	vertices4[0] = position.x + size.x - radius;
	vertices4[1] = position.y + size.y - radius;
	for (int i = 0; i < points; i++) {
		float anglerad = M_PI * ((90 / points) * i) / 180.0f;
		float x = cosf(anglerad) * radius;
		float y = sinf(anglerad) * radius;
		vertices4[2 + i * 2] = position.x + size.x - radius + x;
		vertices4[2 + i * 2 + 1] = position.y + size.y - radius + y;
	}
	vertices4[2 + points * 2] = position.x + size.x - radius;
	vertices4[2 + points * 2 + 1] = position.y + size.y;

	float vertices5[24] = {
		position.x + radius, position.y,
		position.x + radius, position.y + size.y,
		position.x + size.x - radius, position.y + size.y,

		position.x + radius, position.y,
		position.x + size.x - radius, position.y + size.y,
		position.x + size.x - radius, position.y,

		position.x, position.y + radius,
		position.x, position.y + size.y - radius,
		position.x + size.x, position.y + size.y - radius,

		position.x, position.y + radius,
		position.x + size.x, position.y + size.y - radius,
		position.x + size.x, position.y + radius
	};

	glBindVertexArray(m_bigVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bigVBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vsize, vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vsize, sizeof(float) * vsize, vertices2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vsize * 2, sizeof(float) * vsize, vertices3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vsize * 3, sizeof(float) * vsize, vertices4);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vsize * 4, sizeof(float) * 24, vertices5);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vsize2);
	glDrawArrays(GL_TRIANGLE_FAN, vsize2, vsize2);
	glDrawArrays(GL_TRIANGLE_FAN, vsize2 * 2, vsize2);
	glDrawArrays(GL_TRIANGLE_FAN, vsize2 * 3, vsize2);
	glDrawArrays(GL_TRIANGLE_FAN, vsize2 * 4, 12);
	glBindVertexArray(0);

	delete[] vertices1;
	delete[] vertices2;
	delete[] vertices3;
	delete[] vertices4;
}

void glacier::Application::drawText(std::string text, CharacterMap font, glm::vec2 position, glm::vec4 color, float size, int initialHeight) {
	m_textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(m_textShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_textShader.programID, "textColor"), color.r, color.g, color.b, color.a);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_textVAO);

	float scale = size / initialHeight;
	float x = position.x;
	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font[*c];

		if(*c == ' ') {
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
			continue;
		}

		float xpos = x + ch.Bearing.x * scale;
		float ypos = position.y - (ch.Bearing.y) * scale + (ch.LoadedSize * scale);

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos,     ypos,       0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },

			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void glacier::Application::drawTexture(Texture texture, glm::vec2 position, glm::vec2 size, glm::vec4 color) {
	m_textureShader.use();
	glUniformMatrix4fv(glGetUniformLocation(m_textureShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform4f(glGetUniformLocation(m_textureShader.programID, "color"), color.r, color.g, color.b, color.a);

	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);

	float vertices[24] = {
		position.x, 			position.y, 			0.0f, 1.0f,
		position.x, 			position.y + size.y, 	0.0f, 0.0f,
		position.x + size.x, 	position.y + size.y, 	1.0f, 0.0f,
		position.x,			 	position.y,			 	0.0f, 1.0f,
		position.x + size.x, 	position.y + size.y, 	1.0f, 0.0f,
		position.x + size.x, 	position.y,			 	1.0f, 1.0f
	};

	texture.bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 24, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void glacier::Application::render() {
	m_deltaTime = std::chrono::high_resolution_clock::now() - m_frameStart;
	m_time += m_deltaTime.count();
	m_frameStart = std::chrono::high_resolution_clock::now();

	/* ------------ Disable previous shader (to avoid texture errors) ----------- */
	glUseProgram(0);
	
	glm::vec4 cc = Colors::getColorGL(ColorCode::Base0);
	glClearColor(cc.r, cc.g, cc.b, cc.a);
	glClear(GL_COLOR_BUFFER_BIT);
	glFrontFace(GL_CCW);
	
	m_projection = glm::ortho(0.0f, (float)m_windowSize.x, (float)m_windowSize.y, 0.0f);

	//m_colorTest.render();
	m_navigation.render();
	/*glm::vec4 colorr = { std::sin(m_time * 2.0), std::sin(2 + m_time * 2.0), std::sin(4 + m_time * 2.0), 1.0f };
	glm::vec4 colorr1 = { std::sin(2 + m_time * 1.0), std::sin(4 + m_time * 1.0), std::sin(6 + m_time * 1.0), 1.0f };
	glm::vec4 colorr2 = { std::sin(4 + m_time * 4.0), std::sin(6 + m_time * 4.0), std::sin(8 + m_time * 4.0), 1.0f };
	glm::vec4 colorr3 = { std::sin(6 + m_time * 3.0), std::sin(8 + m_time * 3.0), std::sin(10 + m_time * 3.0), 1.0f };
	glm::vec4 colorr4 = { std::sin(8 + m_time * 6.0), std::sin(10 + m_time * 6.0), std::sin(12 + m_time * 6.0), 1.0f };
	drawText("ASK", m_roboto, { 50, 50 }, colorr, 384);
	drawText("ASK...", m_roboto, { 50, 450 }, colorr1, 128);
	drawText("Ask...", m_roboto, { 50, 600 }, colorr2, 90);
	drawText("ask...", m_roboto, { 50, 700 }, colorr3, 40);
	drawText("ask", m_roboto, { 50, 750 }, colorr4, 16);*/

	if(m_mouse.left && !m_mouseFirstFrame.left && !m_mouseHold.left) {
		m_mouseFirstFrame.left = true;
		m_mouseHold.left = true;
	}
	else if(m_mouse.left && m_mouseFirstFrame.left) {
		m_mouseFirstFrame.left = false;
	}
	else if(!m_mouse.left) {
		m_mouseFirstFrame.left = false;
		m_mouseHold.left = false;
	}

	if(m_mouse.middle && !m_mouseFirstFrame.middle && !m_mouseHold.middle) {
		m_mouseFirstFrame.middle = true;
		m_mouseHold.middle = true;
	}
	else if(m_mouse.middle && m_mouseFirstFrame.middle) {
		m_mouseFirstFrame.middle = false;
	}
	else if(!m_mouse.middle) {
		m_mouseFirstFrame.middle = false;
		m_mouseHold.middle = false;
	}

	if(m_mouse.right && !m_mouseFirstFrame.right && !m_mouseHold.right) {
		m_mouseFirstFrame.right = true;
		m_mouseHold.right = true;
	}
	else if(m_mouse.right && m_mouseFirstFrame.right) {
		m_mouseFirstFrame.right = false;
	}
	else if(!m_mouse.right) {
		m_mouseFirstFrame.right = false;
		m_mouseHold.right = false;
	}

	// cursor
	drawRectFill({ m_cursor.x - 5, m_cursor.y - 5 }, { 10, 10 }, Colors::getColorGL(ColorCode::Overlay0));
	
	double fps = 1000.0 / getDelta();
	glm::vec4 fpsColor = glm::mix(Colors::getColorGL(ColorCode::Red), Colors::getColorGL(ColorCode::Green), fps / 60.0f);
	std::stringstream fpss,dtss;
	fpss << "FPS: " << fps;
	dtss << "dt: " << getDelta() << "ms";
	drawText(fpss.str(), m_roboto, { 10, 10 }, fpsColor, 16);
	drawText(dtss.str(), m_roboto, { 10, 10 + 20 }, fpsColor, 16);
}

void glacier::Application::cleanup() {
	LOG(Log::Level::Debug, "glacier::Application::cleanup", "Terminating GLFW");
	glfwTerminate();
}
