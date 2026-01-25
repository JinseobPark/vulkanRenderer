#pragma once

#include "device.h"
#include "pipeline.h"
#include "swapchain.h"
#include "window.h"

#include <memory>
#include <vector>

class App {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();
    ~App();

    // 복사/이동 방지
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int frameIndex, uint32_t imageIndex);

    Window window_{WIDTH, HEIGHT, "Vulkan Renderer"};
    Device device_{window_};
    std::unique_ptr<SwapChain> swapChain_;
    std::unique_ptr<Pipeline> pipeline_;
    VkPipelineLayout pipelineLayout_;
    std::vector<VkCommandBuffer> commandBuffers_;
    uint32_t currentFrame_ = 0;
};
