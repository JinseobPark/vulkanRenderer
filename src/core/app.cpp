#include "app.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace vkr {

struct GlobalUbo {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

App::App() {
    Input::init(window_);
    
    globalPool_ = DescriptorPool::Builder(device_)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
    
    loadGameObjects();
    createUniformBuffers();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
    
    // Initialize ImGui after swapchain is created
    imguiLayer_ = std::make_unique<ImGuiLayer>(
        device_, window_, swapChain_->getRenderPass(), 
        static_cast<uint32_t>(swapChain_->imageCount()));
}

App::~App() {
    vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}

void App::run() {
    std::cout << "Starting Vulkan Renderer..." << std::endl;
    
    while (!window_.shouldClose()) {
        window_.pollEvents();
        Input::update();
        
        float deltaTime = timer_.tick();
        updateCamera(deltaTime);
        
        // Rotate cube
        rotationAngle_ += deltaTime * 45.0f; // 45 degrees per second
        
        drawFrame();
    }

    device_.waitIdle();
}

void App::updateCamera(float deltaTime) {
    // Orbit camera control with mouse
    if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        glm::vec2 delta = Input::getMouseDelta();
        camera_.rotate(-delta.x, -delta.y);
    }
    
    // Zoom with scroll or right mouse + drag
    float scroll = Input::getScrollDelta();
    if (scroll != 0.0f) {
        camera_.zoom(scroll * 2.0f);
    }
    
    // Pan with middle mouse
    if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        glm::vec2 delta = Input::getMouseDelta();
        camera_.pan(-delta.x, delta.y);
    }
    
    // FPS controls with WASD (when in FPS mode)
    if (camera_.getMode() == Camera::Mode::FPS) {
        if (Input::isKeyPressed(GLFW_KEY_W)) camera_.moveForward(deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_S)) camera_.moveForward(-deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_A)) camera_.moveRight(-deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_D)) camera_.moveRight(deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_Q)) camera_.moveUp(-deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_E)) camera_.moveUp(deltaTime);
    }
    
    // Toggle camera mode with Tab
    if (Input::isKeyJustPressed(GLFW_KEY_TAB)) {
        if (camera_.getMode() == Camera::Mode::Orbit) {
            camera_.setMode(Camera::Mode::FPS);
            Input::setCursorMode(GLFW_CURSOR_DISABLED);
        } else {
            camera_.setMode(Camera::Mode::Orbit);
            Input::setCursorMode(GLFW_CURSOR_NORMAL);
        }
    }
    
    // Escape to close
    if (Input::isKeyJustPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window_.getGLFWWindow(), GLFW_TRUE);
    }
    
    camera_.update(deltaTime);
}

void App::loadGameObjects() {
    cubeMesh_ = Mesh::createCube(device_, glm::vec3(1.0f));
    
    // Setup camera
    camera_.lookAt(glm::vec3(3.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera_.setOrbitDistance(5.0f);
}

void App::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(GlobalUbo);
    
    uboBuffers_.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    
    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        uboBuffers_[i] = std::make_unique<Buffer>(
            device_,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        uboBuffers_[i]->map();
    }
    
    globalSetLayout_ = DescriptorSetLayout::Builder(device_)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
        .build();
    
    globalDescriptorSets_.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferInfo = uboBuffers_[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout_, *globalPool_)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets_[i]);
    }
}

void App::createPipelineLayout() {
    VkDescriptorSetLayout setLayout = globalSetLayout_->getDescriptorSetLayout();
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &setLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void App::createPipeline() {
    assert(swapChain_ != nullptr && "Cannot create pipeline before swap chain");
    assert(pipelineLayout_ != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = swapChain_->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout_;
    pipelineConfig.bindingDescriptions = Mesh::Vertex::getBindingDescriptions();
    pipelineConfig.attributeDescriptions = Mesh::Vertex::getAttributeDescriptions();
    
    pipeline_ = std::make_unique<Pipeline>(
        device_,
        "shaders/mesh.vert.spv",
        "shaders/mesh.frag.spv",
        pipelineConfig);
}

void App::createCommandBuffers() {
    commandBuffers_.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device_.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

    if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void App::freeCommandBuffers() {
    vkFreeCommandBuffers(
        device_.device(),
        device_.getCommandPool(),
        static_cast<uint32_t>(commandBuffers_.size()),
        commandBuffers_.data());
    commandBuffers_.clear();
}

void App::recreateSwapChain() {
    auto extent = window_.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = window_.getExtent();
        glfwWaitEvents();
    }
    device_.waitIdle();

    if (swapChain_ == nullptr) {
        swapChain_ = std::make_unique<SwapChain>(device_, extent);
    } else {
        std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain_);
        swapChain_ = std::make_unique<SwapChain>(device_, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*swapChain_.get())) {
            throw std::runtime_error("Swap chain image or depth format has changed!");
        }
    }
    
    // Update camera aspect ratio
    camera_.setAspectRatio(swapChain_->extentAspectRatio());

    createPipeline();
}

void App::recordCommandBuffer(int frameIndex, uint32_t imageIndex) {
    // Start new ImGui frame
    imguiLayer_->newFrame();
    renderImGui();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers_[frameIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain_->getRenderPass();
    renderPassInfo.framebuffer = swapChain_->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain_->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.02f, 0.02f, 0.02f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers_[frameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain_->getSwapChainExtent().width);
    viewport.height = static_cast<float>(swapChain_->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, swapChain_->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers_[frameIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers_[frameIndex], 0, 1, &scissor);

    // Update UBO
    GlobalUbo ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle_), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.view = camera_.getViewMatrix();
    ubo.projection = camera_.getProjectionMatrix();
    
    uboBuffers_[frameIndex]->writeToBuffer(&ubo);

    // Bind pipeline and draw
    pipeline_->bind(commandBuffers_[frameIndex]);
    
    vkCmdBindDescriptorSets(
        commandBuffers_[frameIndex],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout_,
        0, 1,
        &globalDescriptorSets_[frameIndex],
        0, nullptr);
    
    cubeMesh_->bind(commandBuffers_[frameIndex]);
    cubeMesh_->draw(commandBuffers_[frameIndex]);

    // Render ImGui
    imguiLayer_->render(commandBuffers_[frameIndex]);

    vkCmdEndRenderPass(commandBuffers_[frameIndex]);
    if (vkEndCommandBuffer(commandBuffers_[frameIndex]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void App::drawFrame() {
    uint32_t imageIndex;
    auto result = swapChain_->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    recordCommandBuffer(currentFrame_, imageIndex);
    result = swapChain_->submitCommandBuffers(&commandBuffers_[currentFrame_], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        window_.wasWindowResized()) {
        window_.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    currentFrame_ = (currentFrame_ + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void App::renderImGui() {
    // Stats window
    ImGui::Begin("Renderer Stats");
    ImGui::Text("Frame Time: %.3f ms", timer_.getDeltaTime() * 1000.0f);
    ImGui::Text("FPS: %.1f", 1.0f / timer_.getDeltaTime());
    ImGui::Separator();
    ImGui::Text("Camera Mode: %s", camera_.getMode() == Camera::Mode::Orbit ? "Orbit" : "FPS");
    ImGui::Text("Press TAB to toggle camera mode");
    ImGui::End();

    // Demo window (optional)
    if (showDemoWindow_) {
        imguiLayer_->showDemoWindow(&showDemoWindow_);
    }
}

} // namespace vkr
