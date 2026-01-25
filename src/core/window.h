#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include <string>

namespace vkr {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // 복사/이동 방지
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const { return glfwWindowShouldClose(window_); }
    void pollEvents() const { glfwPollEvents(); }
    VkExtent2D getExtent() const;
    bool wasWindowResized() const { return framebufferResized_; }
    void resetWindowResizedFlag() { framebufferResized_ = false; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    GLFWwindow* getGLFWWindow() const { return window_; }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    int width_;
    int height_;
    bool framebufferResized_ = false;
    std::string title_;

    GLFWwindow* window_;
};

} // namespace vkr
