#pragma once

#include "window.h"
#include <glm/glm.hpp>
#include <array>

namespace rs {

class Input {
public:
    static void init(Window& window);
    static void update();

    // Keyboard
    static bool isKeyPressed(int key);
    static bool isKeyJustPressed(int key);
    static bool isKeyJustReleased(int key);

    // Mouse
    static bool isMouseButtonPressed(int button);
    static bool isMouseButtonJustPressed(int button);
    static bool isMouseButtonJustReleased(int button);
    
    static glm::vec2 getMousePosition();
    static glm::vec2 getMouseDelta();
    static float getScrollDelta();

    // Cursor control
    static void setCursorMode(int mode);
    static void setCursorPosition(double x, double y);

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static inline GLFWwindow* window_ = nullptr;
    
    static constexpr int MAX_KEYS = 512;
    static constexpr int MAX_BUTTONS = 8;
    
    static inline std::array<bool, MAX_KEYS> currentKeys_{};
    static inline std::array<bool, MAX_KEYS> previousKeys_{};
    
    static inline std::array<bool, MAX_BUTTONS> currentButtons_{};
    static inline std::array<bool, MAX_BUTTONS> previousButtons_{};
    
    static inline glm::vec2 mousePosition_{0.0f};
    static inline glm::vec2 previousMousePosition_{0.0f};
    static inline glm::vec2 mouseDelta_{0.0f};
    static inline float scrollDelta_ = 0.0f;
    static inline bool firstMouse_ = true;
};

} // namespace rs
