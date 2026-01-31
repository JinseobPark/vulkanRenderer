#pragma once

#include "../vulkan/device.h"
#include "../vulkan/swapchain.h"
#include "../core/window.h"

#include <imgui.h>

namespace rs {

class ImGuiLayer {
public:
    ImGuiLayer(Device& device, Window& window, VkRenderPass renderPass, uint32_t imageCount);
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;

    void newFrame();
    void render(VkCommandBuffer commandBuffer);

    // For demo/testing
    void showDemoWindow(bool* open = nullptr);

private:
    void init(VkRenderPass renderPass, uint32_t imageCount);
    void setupStyle();

    Device& device_;
    VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;
};

} // namespace rs
