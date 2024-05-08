
#pragma once

#include "glfw.hpp"
#include "text.h"
#include "shader.h"
#include "colortest_element.h"
#include "navigation_element.h"
#include "texture.h"
#include "keyevent.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <chrono>

namespace glacier {

    struct MouseButtons {
        bool left;
        bool middle;
        bool right;
    };

    typedef std::pair<int, int> Key;

    struct KeyboardButtons {
        std::list<Key> current;
        int repeat;

        const char* keyToChar(Key key);
    };

    class Application {
    public:
        Application() {};
        void run() {
            initWindow();
            
            loadFonts();
            loadShaders();
            setupBuffers();
            
            mainLoop();
            cleanup();
        }

        void drawLine(glm::vec2 from, glm::vec2 to, glm::vec4 color, float width = 1.0f);
        void drawCircle(glm::vec2 position, float radius, glm::vec4 color, float width = 1.0f, int points = 16);
        void drawCircleFill(glm::vec2 position, glm::vec4 color, float radius, int points = 16);
        void drawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color, float width = 1.0f);
        void drawRectFill(glm::vec2 position, glm::vec2 size, glm::vec4 color);
        void drawRectRounded(glm::vec2 position, glm::vec2 size, glm::vec4 color, float radius, int points = 8);
        void drawRectRoundedFill(glm::vec2 position, glm::vec2 size, glm::vec4 color, float radius, int points = 8);
        void drawText(std::string text, CharacterMap font, glm::vec2 position, glm::vec4 color, float size, int initialHeight = 1024);
        void drawTexture(Texture texture, glm::vec2 position, glm::vec2 size, glm::vec4 color);

        bool checkCollisionRect(glm::vec2 point, glm::vec2 rectPos, glm::vec2 rectSize) {
            return  point.x >= rectPos.x &&
                    point.x <= rectPos.x + rectSize.x &&
                    point.y >= rectPos.y &&
                    point.y <= rectPos.y + rectSize.y;
        };

        bool checkCollisionCircle(glm::vec2 point, glm::vec2 circlePos, float radius) {
            float distance = std::sqrt(std::pow(point.x - circlePos.x, 2) + std::pow(point.y - circlePos.y, 2));
            return distance <= radius;
        };

        void setColor(glm::vec4 color) { setColorGL(color / 255.0f); };
        void setColorGL(glm::vec4 color) { glColor3f(color.r, color.g, color.b); };
        
        glm::ivec2 getCursor() const { return m_cursor; }
        glm::vec2 getWindowSize() const { return m_windowSize; }
        CharacterMap getRoboto() const { return m_roboto; }
        double getDelta() const { return m_deltaTime.count() * 1000; }
        double getDeltaSeconds() const { return m_deltaTime.count(); }
        MouseButtons getMouseButtons() const { return m_mouse; };
        MouseButtons getMouseButtonsFirstFrame() const { return m_mouseFirstFrame; }

        void setKeyCharSubscriber(KeyCharEventFunc subscriber, void* userParam, std::string id) {
            m_keyEventSubscriber = subscriber;
            m_keyEventSubscriberUserParam = userParam;
            m_keyEventSubscriberID = id;
        };

        void unsetKeyCharSubscriber() {
            m_keyEventSubscriber = nullptr;
            m_keyEventSubscriberUserParam = nullptr;
            m_keyEventSubscriberID = "";
        };

    private:
        void initWindow();
        
        // events
        void windowRefreshCallback(GLFWwindow* window);
        void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void keyCallback(GLFWwindow* window, int button, int action, int mods);
        // OpenGL debug callback
        void debugMessageCallback(GLenum source, GLenum type, GLuint id,
                GLenum severity, GLsizei length, const GLchar* message, const void* userParam) const;

        void mainLoop();
        void loadFonts();
        void loadShaders();
        void setupBuffers();
        void render();

        void cleanup();

        GLFWwindow* m_window = nullptr;
        glm::ivec2 m_windowSize = { 800, 800 };
        glm::vec2 m_cursor = { 0, 0 };
        glm::mat4 m_projection;
        std::chrono::duration<double> m_deltaTime;
        std::chrono::high_resolution_clock::time_point m_frameStart;
        double m_time;

        // KeyEventLoseFunc m_keyEventLoseSubscriber;
        KeyCharEventFunc m_keyEventSubscriber = nullptr;
        void* m_keyEventSubscriberUserParam = nullptr;
        std::string m_keyEventSubscriberID = "";

        // KeyboardButtons m_keyboard;
        MouseButtons m_mouse;
        // states if its the first frame of a button being pressed
        // can be used for buttons or sum
        MouseButtons m_mouseFirstFrame;
        MouseButtons m_mouseHold;

        // Text rendering
        CharacterMap m_roboto;
        unsigned int m_textVAO, m_textVBO = 0;
        Shader m_textShader;

        // Regular rendering
        unsigned int m_lineVAO, m_lineVBO = 0;
        unsigned int m_bigVAO, m_bigVBO = 0;
        unsigned int m_textureVAO, m_textureVBO = 0;
        Shader m_regularShader;
        Shader m_textureShader;

        // Elements
        ColorTestElement m_colorTest;
        NavigationElement m_navigation;
    };

}
