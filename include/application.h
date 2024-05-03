
#pragma once

#include "glfw.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace glacier {

    class Application {
    public:
        void run() {
            initWindow();
            mainLoop();
            cleanup();
        }

        void drawLine(glm::vec2 from, glm::vec2 to, float width = 1.0f);
        void drawCircle(glm::vec2 position, float radius, int points = 16);
        void drawCircleFill(glm::vec2 position, glm::vec3 color, float radius, int points = 16);
        void drawRect(glm::vec2 position, glm::vec2 size);
        void drawRectFill(glm::vec2 position, glm::vec2 size, glm::vec3 color);
        void drawRectRounded(glm::vec2 position, glm::vec2 size, float radius, int points = 8);
        void drawRectRoundedFill(glm::vec2 position, glm::vec2 size, glm::vec3 color, float radius, int points = 8);
        void setColor(glm::vec3 color) { setColorGL(color / 255.0f); };
        void setColorGL(glm::vec3 color) { glColor3f(color.r, color.g, color.b); };

        glm::ivec2 getCursor() { return m_cursor; }
        glm::vec2 getWindowSize() { return m_windowSize; }

    private:
        void initWindow();

        void mainLoop();
        void windowRefreshCallback(GLFWwindow* window);
        void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        void processWindowSize();
        void processCursorPosition();
        void render();

        void cleanup();

        GLFWwindow* m_window;
        glm::ivec2 m_windowSize = { 800, 800 };
        glm::vec2 m_cursor = { 0, 0 };
    };

}
