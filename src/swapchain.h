#pragma once

#include "device.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

class SwapChain {
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    SwapChain(Device& device, VkExtent2D windowExtent);
    SwapChain(Device& device, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
    ~SwapChain();

    // 복사/이동 방지
    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    VkFramebuffer getFrameBuffer(int index) const { return swapChainFramebuffers_[index]; }
    VkRenderPass getRenderPass() const { return renderPass_; }
    VkImageView getImageView(int index) const { return swapChainImageViews_[index]; }
    size_t imageCount() const { return swapChainImages_.size(); }
    VkFormat getSwapChainImageFormat() const { return swapChainImageFormat_; }
    VkExtent2D getSwapChainExtent() const { return swapChainExtent_; }
    uint32_t width() const { return swapChainExtent_.width; }
    uint32_t height() const { return swapChainExtent_.height; }

    float extentAspectRatio() const {
        return static_cast<float>(swapChainExtent_.width) / static_cast<float>(swapChainExtent_.height);
    }

    VkFormat findDepthFormat();

    VkResult acquireNextImage(uint32_t* imageIndex);
    VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

    bool compareSwapFormats(const SwapChain& swapChain) const {
        return swapChain.swapChainImageFormat_ == swapChainImageFormat_;
    }

private:
    void init();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkFormat swapChainImageFormat_;
    VkExtent2D swapChainExtent_;

    std::vector<VkFramebuffer> swapChainFramebuffers_;
    VkRenderPass renderPass_;

    std::vector<VkImage> swapChainImages_;
    std::vector<VkImageView> swapChainImageViews_;

    Device& device_;
    VkExtent2D windowExtent_;

    VkSwapchainKHR swapChain_;
    std::shared_ptr<SwapChain> oldSwapChain_;

    std::vector<VkSemaphore> imageAvailableSemaphores_;
    std::vector<VkSemaphore> renderFinishedSemaphores_;
    std::vector<VkFence> inFlightFences_;
    std::vector<VkFence> imagesInFlight_;
    size_t currentFrame_ = 0;
};
