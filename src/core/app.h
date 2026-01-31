#pragma once

#include "window.h"
#include "input.h"
#include "camera.h"
#include "timer.h"
#include "../vulkan/device.h"
#include "../vulkan/swapchain.h"
#include "../vulkan/pipeline.h"
#include "../vulkan/descriptor.h"
#include "../vulkan/buffer.h"
#include "../resource/mesh.h"
#include "../ui/imgui_layer.h"

#include <memory>
#include <vector>

namespace rs {

class App {
public:
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;

    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    void loadGameObjects();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int frameIndex, uint32_t imageIndex);
    void updateCamera(float deltaTime);
    void createUniformBuffers();
    void renderImGui();

    Window window_{WIDTH, HEIGHT, "Vulkan Fluid & Cloth Renderer"};
    Device device_{window_};
    std::unique_ptr<SwapChain> swapChain_;
    std::unique_ptr<Pipeline> pipeline_;
    VkPipelineLayout pipelineLayout_;
    std::vector<VkCommandBuffer> commandBuffers_;
    
    std::unique_ptr<DescriptorPool> globalPool_;
    std::unique_ptr<DescriptorSetLayout> globalSetLayout_;
    std::vector<VkDescriptorSet> globalDescriptorSets_;
    std::vector<std::unique_ptr<Buffer>> uboBuffers_;
    
    std::unique_ptr<ImGuiLayer> imguiLayer_;
    
    Camera camera_;
    Timer timer_;
    
    std::unique_ptr<Mesh> cubeMesh_;
    
    uint32_t currentFrame_ = 0;
    float rotationAngle_ = 0.0f;
    bool showDemoWindow_ = true;
};

} // namespace rs
