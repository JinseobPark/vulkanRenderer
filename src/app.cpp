#include "app.h"

#include <array>
#include <cassert>
#include <stdexcept>

App::App() {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

App::~App() {
    vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}

void App::run() {
    while (!window_.shouldClose()) {
        window_.pollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device_.device());
}

void App::loadModels() {
    // 향후 모델 로딩 로직 추가
}

void App::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
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
    pipeline_ = std::make_unique<Pipeline>(
        device_,
        "shaders/triangle.vert.spv",
        "shaders/triangle.frag.spv",
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
    vkDeviceWaitIdle(device_.device());

    if (swapChain_ == nullptr) {
        swapChain_ = std::make_unique<SwapChain>(device_, extent);
    } else {
        std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain_);
        swapChain_ = std::make_unique<SwapChain>(device_, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*swapChain_.get())) {
            throw std::runtime_error("Swap chain image format has changed!");
        }
    }

    createPipeline();
}

void App::recordCommandBuffer(int frameIndex, uint32_t imageIndex) {
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

    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
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

    pipeline_->bind(commandBuffers_[frameIndex]);
    vkCmdDraw(commandBuffers_[frameIndex], 3, 1, 0, 0);

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
