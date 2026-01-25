#include "window.h"

#include <stdexcept>

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height), title_(title) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);
}

Window::~Window() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, window_, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

VkExtent2D Window::getExtent() const {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->framebufferResized_ = true;
    windowPtr->width_ = width;
    windowPtr->height_ = height;
}
