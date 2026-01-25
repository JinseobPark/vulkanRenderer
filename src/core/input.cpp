#include "input.h"

namespace vkr {

void Input::init(Window& window) {
    window_ = window.getGLFWWindow();
    
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, cursorPosCallback);
    glfwSetScrollCallback(window_, scrollCallback);
    
    // Initialize mouse position
    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    mousePosition_ = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    previousMousePosition_ = mousePosition_;
}

void Input::update() {
    // Copy current state to previous
    previousKeys_ = currentKeys_;
    previousButtons_ = currentButtons_;
    previousMousePosition_ = mousePosition_;
    
    // Reset per-frame values
    scrollDelta_ = 0.0f;
    mouseDelta_ = glm::vec2(0.0f);
}

bool Input::isKeyPressed(int key) {
    if (key < 0 || key >= MAX_KEYS) return false;
    return currentKeys_[key];
}

bool Input::isKeyJustPressed(int key) {
    if (key < 0 || key >= MAX_KEYS) return false;
    return currentKeys_[key] && !previousKeys_[key];
}

bool Input::isKeyJustReleased(int key) {
    if (key < 0 || key >= MAX_KEYS) return false;
    return !currentKeys_[key] && previousKeys_[key];
}

bool Input::isMouseButtonPressed(int button) {
    if (button < 0 || button >= MAX_BUTTONS) return false;
    return currentButtons_[button];
}

bool Input::isMouseButtonJustPressed(int button) {
    if (button < 0 || button >= MAX_BUTTONS) return false;
    return currentButtons_[button] && !previousButtons_[button];
}

bool Input::isMouseButtonJustReleased(int button) {
    if (button < 0 || button >= MAX_BUTTONS) return false;
    return !currentButtons_[button] && previousButtons_[button];
}

glm::vec2 Input::getMousePosition() {
    return mousePosition_;
}

glm::vec2 Input::getMouseDelta() {
    return mouseDelta_;
}

float Input::getScrollDelta() {
    return scrollDelta_;
}

void Input::setCursorMode(int mode) {
    glfwSetInputMode(window_, GLFW_CURSOR, mode);
}

void Input::setCursorPosition(double x, double y) {
    glfwSetCursorPos(window_, x, y);
    mousePosition_ = glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= MAX_KEYS) return;
    
    if (action == GLFW_PRESS) {
        currentKeys_[key] = true;
    } else if (action == GLFW_RELEASE) {
        currentKeys_[key] = false;
    }
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button < 0 || button >= MAX_BUTTONS) return;
    
    if (action == GLFW_PRESS) {
        currentButtons_[button] = true;
    } else if (action == GLFW_RELEASE) {
        currentButtons_[button] = false;
    }
}

void Input::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    glm::vec2 newPos(static_cast<float>(xpos), static_cast<float>(ypos));
    
    if (firstMouse_) {
        previousMousePosition_ = newPos;
        firstMouse_ = false;
    }
    
    mouseDelta_ = newPos - previousMousePosition_;
    mousePosition_ = newPos;
}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollDelta_ = static_cast<float>(yoffset);
}

} // namespace vkr
