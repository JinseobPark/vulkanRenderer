#pragma once

#include "../core/window.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace rs {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> computeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
    
    bool hasCompute() const {
        return computeFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Device {
public:
#ifdef ENABLE_VALIDATION_LAYERS
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif

    Device(Window& window);
    ~Device();

    // 복사/이동 방지
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    // Getters
    VkCommandPool getCommandPool() const { return commandPool_; }
    VkDevice device() const { return device_; }
    VkInstance instance() const { return instance_; }
    VkSurfaceKHR surface() const { return surface_; }
    VkQueue graphicsQueue() const { return graphicsQueue_; }
    VkQueue computeQueue() const { return computeQueue_; }
    VkQueue presentQueue() const { return presentQueue_; }
    VkPhysicalDevice physicalDevice() const { return physicalDevice_; }
    VkPhysicalDeviceProperties getProperties() const { return properties_; }

    SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice_); }
    QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice_); }
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat findSupportedFormat(
        const std::vector<VkFormat>& candidates,
        VkImageTiling tiling,
        VkFormatFeatureFlags features);

    // Buffer creation
    void createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    // Image creation
    void createImage(
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkImage& image,
        VkDeviceMemory& imageMemory,
        uint32_t mipLevels = 1);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels = 1);
    
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1);

    // Command buffers
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void waitIdle() { vkDeviceWaitIdle(device_); }

private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();

    bool isDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkInstance instance_;
    VkDebugUtilsMessengerEXT debugMessenger_;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties properties_;
    Window& window_;
    VkCommandPool commandPool_;

    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphicsQueue_;
    VkQueue computeQueue_;
    VkQueue presentQueue_;

    const std::vector<const char*> validationLayers_ = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions_ = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

} // namespace rs
