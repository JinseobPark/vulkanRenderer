# Vulkan API Reference

본 문서는 `main.cpp`에서 사용된 Vulkan API들을 스터디 목적으로 정리한 문서입니다.

---

## 목차
1. [Instance 생성 및 디버깅](#1-instance-생성-및-디버깅)
2. [Physical Device 선택](#2-physical-device-선택)
3. [Logical Device 및 Queue](#3-logical-device-및-queue)
4. [Surface & Swapchain](#4-surface--swapchain)
5. [Image & ImageView](#5-image--imageview)
6. [Render Pass](#6-render-pass)
7. [Graphics Pipeline](#7-graphics-pipeline)
8. [Framebuffer](#8-framebuffer)
9. [Command Pool & Command Buffer](#9-command-pool--command-buffer)
10. [Synchronization (동기화)](#10-synchronization-동기화)
11. [Drawing & Presentation](#11-drawing--presentation)

---

## 1. Instance 생성 및 디버깅

### `vkCreateInstance`
Vulkan 인스턴스를 생성합니다. 모든 Vulkan 작업의 시작점입니다.

**사용 예시:**
```cpp
VkInstanceCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
createInfo.pApplicationInfo = &appInfo;
vkCreateInstance(&createInfo, nullptr, &instance);
```

**주요 멤버:**
- `pApplicationInfo`: 애플리케이션 정보
- `enabledLayerCount`: 활성화할 레이어 수 (validation layer 등)
- `ppEnabledLayerNames`: 레이어 이름 배열
- `enabledExtensionCount`: 활성화할 확장 수
- `ppEnabledExtensionNames`: 확장 이름 배열

---

### `vkGetInstanceProcAddr`
인스턴스 레벨 함수 포인터를 가져옵니다.

**사용 예시:**
```cpp
auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    instance, "vkCreateDebugUtilsMessengerEXT");
```

**용도:** 확장 함수나 디버그 유틸리티 함수 로드

---

### `vkEnumerateInstanceLayerProperties`
사용 가능한 레이어 목록을 조회합니다.

**사용 예시:**
```cpp
uint32_t layerCount;
vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
std::vector<VkLayerProperties> availableLayers(layerCount);
vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
```

**용도:** Validation Layer 지원 여부 확인

---

### `vkDestroyInstance`
Vulkan 인스턴스를 파괴합니다.

**사용 예시:**
```cpp
vkDestroyInstance(instance, nullptr);
```

**주의:** 인스턴스 생성 이후 생성된 모든 자원을 먼저 파괴해야 합니다.

---

## 2. Physical Device 선택

### `vkEnumeratePhysicalDevices`
시스템에서 사용 가능한 GPU(Physical Device) 목록을 가져옵니다.

**사용 예시:**
```cpp
uint32_t deviceCount = 0;
vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
std::vector<VkPhysicalDevice> devices(deviceCount);
vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
```

---

### `vkGetPhysicalDeviceQueueFamilyProperties`
Physical Device의 Queue Family 속성을 조회합니다.

**사용 예시:**
```cpp
uint32_t queueFamilyCount = 0;
vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
```

**주요 속성:**
- `queueFlags`: `VK_QUEUE_GRAPHICS_BIT`, `VK_QUEUE_COMPUTE_BIT` 등
- `queueCount`: 해당 패밀리에서 사용 가능한 큐 개수

---

### `vkGetPhysicalDeviceSurfaceSupportKHR`
Physical Device가 특정 Surface에 프레젠테이션을 지원하는지 확인합니다.

**사용 예시:**
```cpp
VkBool32 presentSupport = false;
vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamilyIndex, surface, &presentSupport);
```

---

### `vkEnumerateDeviceExtensionProperties`
Physical Device에서 지원하는 확장 목록을 조회합니다.

**사용 예시:**
```cpp
uint32_t extensionCount;
vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
std::vector<VkExtensionProperties> availableExtensions(extensionCount);
vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
```

**주요 확장:**
- `VK_KHR_SWAPCHAIN_EXTENSION_NAME`: 스왑체인 지원

---

### `vkGetPhysicalDeviceSurfaceCapabilitiesKHR`
Surface의 기능(capabilities)을 조회합니다.

**사용 예시:**
```cpp
VkSurfaceCapabilitiesKHR capabilities;
vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
```

**주요 정보:**
- `minImageCount`, `maxImageCount`: 이미지 개수 범위
- `currentExtent`: 현재 Surface 크기
- `minImageExtent`, `maxImageExtent`: 이미지 크기 범위

---

### `vkGetPhysicalDeviceSurfaceFormatsKHR`
Surface에서 지원하는 이미지 포맷 목록을 가져옵니다.

**사용 예시:**
```cpp
uint32_t formatCount;
vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
std::vector<VkSurfaceFormatKHR> formats(formatCount);
vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());
```

**주요 포맷:**
- `VK_FORMAT_B8G8R8A8_SRGB`: BGRA 8비트 sRGB
- `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR`: sRGB 색 공간

---

### `vkGetPhysicalDeviceSurfacePresentModesKHR`
Surface에서 지원하는 프레젠테이션 모드를 가져옵니다.

**사용 예시:**
```cpp
uint32_t presentModeCount;
vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
std::vector<VkPresentModeKHR> presentModes(presentModeCount);
vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data());
```

**주요 모드:**
- `VK_PRESENT_MODE_FIFO_KHR`: V-Sync (항상 지원)
- `VK_PRESENT_MODE_MAILBOX_KHR`: Triple buffering
- `VK_PRESENT_MODE_IMMEDIATE_KHR`: 즉시 표시 (티어링 가능)

---

## 3. Logical Device 및 Queue

### `vkCreateDevice`
Logical Device를 생성합니다. Physical Device와 상호작용하는 인터페이스입니다.

**사용 예시:**
```cpp
VkDeviceCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
createInfo.pQueueCreateInfos = queueCreateInfos.data();
createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
createInfo.pEnabledFeatures = &deviceFeatures;
vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
```

---

### `vkGetDeviceQueue`
생성된 Logical Device에서 Queue를 가져옵니다.

**사용 예시:**
```cpp
vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &graphicsQueue);
```

---

### `vkDestroyDevice`
Logical Device를 파괴합니다.

**사용 예시:**
```cpp
vkDestroyDevice(device, nullptr);
```

---

### `vkDeviceWaitIdle`
Device의 모든 작업이 완료될 때까지 대기합니다.

**사용 예시:**
```cpp
vkDeviceWaitIdle(device);
```

**용도:** 리소스 재생성 전 동기화

---

## 4. Surface & Swapchain

### `vkDestroySurfaceKHR`
Vulkan Surface를 파괴합니다.

**사용 예시:**
```cpp
vkDestroySurfaceKHR(instance, surface, nullptr);
```

**참고:** Surface는 GLFW를 통해 생성됨 (`glfwCreateWindowSurface`)

---

### `vkCreateSwapchainKHR`
Swapchain을 생성합니다. 렌더링된 이미지를 화면에 표시하기 위한 이미지 큐입니다.

**사용 예시:**
```cpp
VkSwapchainCreateInfoKHR createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
createInfo.surface = surface;
createInfo.minImageCount = imageCount;
createInfo.imageFormat = surfaceFormat.format;
createInfo.imageColorSpace = surfaceFormat.colorSpace;
createInfo.imageExtent = extent;
createInfo.imageArrayLayers = 1;
createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
```

**주요 멤버:**
- `imageSharingMode`: `VK_SHARING_MODE_EXCLUSIVE` or `VK_SHARING_MODE_CONCURRENT`
- `presentMode`: 프레젠테이션 모드
- `clipped`: 가려진 픽셀 렌더링 스킵 여부

---

### `vkGetSwapchainImagesKHR`
Swapchain에 포함된 이미지를 가져옵니다.

**사용 예시:**
```cpp
uint32_t imageCount;
vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
std::vector<VkImage> swapChainImages(imageCount);
vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
```

---

### `vkDestroySwapchainKHR`
Swapchain을 파괴합니다.

**사용 예시:**
```cpp
vkDestroySwapchainKHR(device, swapChain, nullptr);
```

---

### `vkAcquireNextImageKHR`
Swapchain에서 다음 렌더링 가능한 이미지를 가져옵니다.

**사용 예시:**
```cpp
uint32_t imageIndex;
VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,
    imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
```

**반환값:**
- `VK_SUCCESS`: 성공
- `VK_ERROR_OUT_OF_DATE_KHR`: Swapchain 재생성 필요 (창 크기 변경 등)
- `VK_SUBOPTIMAL_KHR`: 사용 가능하지만 최적이 아님

---

## 5. Image & ImageView

### `vkCreateImageView`
이미지 뷰를 생성합니다. 이미지를 파이프라인에서 사용하기 위한 인터페이스입니다.

**사용 예시:**
```cpp
VkImageViewCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
createInfo.image = swapChainImages[i];
createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
createInfo.format = swapChainImageFormat;
createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
createInfo.subresourceRange.baseMipLevel = 0;
createInfo.subresourceRange.levelCount = 1;
createInfo.subresourceRange.baseArrayLayer = 0;
createInfo.subresourceRange.layerCount = 1;
vkCreateImageView(device, &createInfo, nullptr, &imageView);
```

**주요 멤버:**
- `viewType`: 이미지 뷰 타입 (1D, 2D, 3D, Cube 등)
- `format`: 이미지 포맷
- `components`: 색상 채널 매핑
- `subresourceRange`: 이미지의 어느 부분을 사용할지

---

### `vkDestroyImageView`
이미지 뷰를 파괴합니다.

**사용 예시:**
```cpp
vkDestroyImageView(device, imageView, nullptr);
```

---

## 6. Render Pass

### `vkCreateRenderPass`
Render Pass를 생성합니다. 렌더링 작업의 구조를 정의합니다.

**사용 예시:**
```cpp
VkAttachmentDescription colorAttachment{};
colorAttachment.format = swapChainImageFormat;
colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

VkSubpassDescription subpass{};
subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
subpass.colorAttachmentCount = 1;
subpass.pColorAttachments = &colorAttachmentRef;

VkRenderPassCreateInfo renderPassInfo{};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
renderPassInfo.attachmentCount = 1;
renderPassInfo.pAttachments = &colorAttachment;
renderPassInfo.subpassCount = 1;
renderPassInfo.pSubpasses = &subpass;
vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
```

**주요 개념:**
- **Attachment**: 렌더링 대상 (color, depth 등)
- **Subpass**: 렌더링 작업 단계
- **Dependency**: Subpass 간 의존성

**loadOp/storeOp:**
- `VK_ATTACHMENT_LOAD_OP_CLEAR`: 시작 시 클리어
- `VK_ATTACHMENT_LOAD_OP_LOAD`: 기존 내용 로드
- `VK_ATTACHMENT_STORE_OP_STORE`: 결과 저장
- `VK_ATTACHMENT_STORE_OP_DONT_CARE`: 결과 무시

---

### `vkDestroyRenderPass`
Render Pass를 파괴합니다.

**사용 예시:**
```cpp
vkDestroyRenderPass(device, renderPass, nullptr);
```

---

## 7. Graphics Pipeline

### `vkCreateShaderModule`
셰이더 모듈을 생성합니다. SPIR-V 바이트코드에서 생성합니다.

**사용 예시:**
```cpp
VkShaderModuleCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
createInfo.codeSize = code.size();
createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
```

---

### `vkDestroyShaderModule`
셰이더 모듈을 파괴합니다.

**사용 예시:**
```cpp
vkDestroyShaderModule(device, shaderModule, nullptr);
```

**참고:** 파이프라인 생성 후 즉시 파괴해도 됨

---

### `vkCreatePipelineLayout`
파이프라인 레이아웃을 생성합니다. 셰이더 리소스(uniform, push constant) 구조를 정의합니다.

**사용 예시:**
```cpp
VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
pipelineLayoutInfo.setLayoutCount = 0; // 디스크립터 세트 레이아웃 수
pipelineLayoutInfo.pushConstantRangeCount = 0;
vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
```

---

### `vkDestroyPipelineLayout`
파이프라인 레이아웃을 파괴합니다.

**사용 예시:**
```cpp
vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
```

---

### `vkCreateGraphicsPipelines`
Graphics Pipeline을 생성합니다. 렌더링 파이프라인의 모든 단계를 구성합니다.

**사용 예시:**
```cpp
VkGraphicsPipelineCreateInfo pipelineInfo{};
pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
pipelineInfo.stageCount = 2;
pipelineInfo.pStages = shaderStages;
pipelineInfo.pVertexInputState = &vertexInputInfo;
pipelineInfo.pInputAssemblyState = &inputAssembly;
pipelineInfo.pViewportState = &viewportState;
pipelineInfo.pRasterizationState = &rasterizer;
pipelineInfo.pMultisampleState = &multisampling;
pipelineInfo.pColorBlendState = &colorBlending;
pipelineInfo.pDynamicState = &dynamicState;
pipelineInfo.layout = pipelineLayout;
pipelineInfo.renderPass = renderPass;
pipelineInfo.subpass = 0;
vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
```

**주요 스테이지:**
1. **Vertex Input State**: 정점 데이터 구조
2. **Input Assembly State**: 정점을 어떻게 조립할지 (삼각형, 선 등)
3. **Viewport State**: 뷰포트 및 시저 영역
4. **Rasterization State**: 래스터화 설정
5. **Multisample State**: 멀티샘플링 설정
6. **Color Blend State**: 색상 블렌딩 설정
7. **Dynamic State**: 동적으로 변경 가능한 상태

---

### `vkDestroyPipeline`
Graphics Pipeline을 파괴합니다.

**사용 예시:**
```cpp
vkDestroyPipeline(device, graphicsPipeline, nullptr);
```

---

## 8. Framebuffer

### `vkCreateFramebuffer`
Framebuffer를 생성합니다. Render Pass의 attachment를 실제 이미지 뷰와 연결합니다.

**사용 예시:**
```cpp
VkFramebufferCreateInfo framebufferInfo{};
framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
framebufferInfo.renderPass = renderPass;
framebufferInfo.attachmentCount = 1;
framebufferInfo.pAttachments = attachments;
framebufferInfo.width = swapChainExtent.width;
framebufferInfo.height = swapChainExtent.height;
framebufferInfo.layers = 1;
vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer);
```

---

### `vkDestroyFramebuffer`
Framebuffer를 파괴합니다.

**사용 예시:**
```cpp
vkDestroyFramebuffer(device, framebuffer, nullptr);
```

---

## 9. Command Pool & Command Buffer

### `vkCreateCommandPool`
Command Pool을 생성합니다. Command Buffer 할당을 위한 메모리 풀입니다.

**사용 예시:**
```cpp
VkCommandPoolCreateInfo poolInfo{};
poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
poolInfo.queueFamilyIndex = queueFamilyIndex;
vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
```

**플래그:**
- `VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT`: 개별 커맨드 버퍼 리셋 가능
- `VK_COMMAND_POOL_CREATE_TRANSIENT_BIT`: 짧은 수명의 커맨드 버퍼

---

### `vkDestroyCommandPool`
Command Pool을 파괴합니다. 할당된 모든 Command Buffer도 함께 해제됩니다.

**사용 예시:**
```cpp
vkDestroyCommandPool(device, commandPool, nullptr);
```

---

### `vkAllocateCommandBuffers`
Command Buffer를 할당합니다.

**사용 예시:**
```cpp
VkCommandBufferAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
allocInfo.commandPool = commandPool;
allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
allocInfo.commandBufferCount = commandBuffers.size();
vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
```

**레벨:**
- `VK_COMMAND_BUFFER_LEVEL_PRIMARY`: Queue에 직접 제출 가능
- `VK_COMMAND_BUFFER_LEVEL_SECONDARY`: Primary에서 호출됨

---

### `vkBeginCommandBuffer`
Command Buffer 기록을 시작합니다.

**사용 예시:**
```cpp
VkCommandBufferBeginInfo beginInfo{};
beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
vkBeginCommandBuffer(commandBuffer, &beginInfo);
```

---

### `vkEndCommandBuffer`
Command Buffer 기록을 종료합니다.

**사용 예시:**
```cpp
vkEndCommandBuffer(commandBuffer);
```

---

### `vkResetCommandBuffer`
Command Buffer를 재사용을 위해 리셋합니다.

**사용 예시:**
```cpp
vkResetCommandBuffer(commandBuffer, 0);
```

---

### `vkCmdBeginRenderPass`
Render Pass를 시작합니다.

**사용 예시:**
```cpp
VkRenderPassBeginInfo renderPassInfo{};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
renderPassInfo.renderPass = renderPass;
renderPassInfo.framebuffer = framebuffer;
renderPassInfo.renderArea.offset = {0, 0};
renderPassInfo.renderArea.extent = swapChainExtent;

VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
renderPassInfo.clearValueCount = 1;
renderPassInfo.pClearValues = &clearColor;

vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
```

---

### `vkCmdEndRenderPass`
Render Pass를 종료합니다.

**사용 예시:**
```cpp
vkCmdEndRenderPass(commandBuffer);
```

---

### `vkCmdBindPipeline`
Graphics Pipeline을 바인딩합니다.

**사용 예시:**
```cpp
vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
```

---

### `vkCmdSetViewport`
뷰포트를 동적으로 설정합니다.

**사용 예시:**
```cpp
VkViewport viewport{};
viewport.x = 0.0f;
viewport.y = 0.0f;
viewport.width = static_cast<float>(swapChainExtent.width);
viewport.height = static_cast<float>(swapChainExtent.height);
viewport.minDepth = 0.0f;
viewport.maxDepth = 1.0f;
vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
```

---

### `vkCmdSetScissor`
시저(클리핑 영역)를 동적으로 설정합니다.

**사용 예시:**
```cpp
VkRect2D scissor{};
scissor.offset = {0, 0};
scissor.extent = swapChainExtent;
vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
```

---

### `vkCmdDraw`
정점 데이터를 사용하여 그리기 명령을 실행합니다.

**사용 예시:**
```cpp
vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
// 예: vkCmdDraw(commandBuffer, 3, 1, 0, 0); // 삼각형 3개 정점
```

**매개변수:**
- `vertexCount`: 그릴 정점 개수
- `instanceCount`: 인스턴스 개수
- `firstVertex`: 첫 정점 인덱스
- `firstInstance`: 첫 인스턴스 인덱스

---

## 10. Synchronization (동기화)

### `vkCreateSemaphore`
Semaphore를 생성합니다. GPU 내부 동기화에 사용됩니다.

**사용 예시:**
```cpp
VkSemaphoreCreateInfo semaphoreInfo{};
semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore);
```

**용도:**
- `imageAvailableSemaphore`: 이미지가 사용 가능해질 때까지 대기
- `renderFinishedSemaphore`: 렌더링 완료 신호

---

### `vkDestroySemaphore`
Semaphore를 파괴합니다.

**사용 예시:**
```cpp
vkDestroySemaphore(device, semaphore, nullptr);
```

---

### `vkCreateFence`
Fence를 생성합니다. CPU-GPU 동기화에 사용됩니다.

**사용 예시:**
```cpp
VkFenceCreateInfo fenceInfo{};
fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // 시그널 상태로 시작
vkCreateFence(device, &fenceInfo, nullptr, &fence);
```

---

### `vkDestroyFence`
Fence를 파괴합니다.

**사용 예시:**
```cpp
vkDestroyFence(device, fence, nullptr);
```

---

### `vkWaitForFences`
하나 이상의 Fence가 시그널될 때까지 CPU가 대기합니다.

**사용 예시:**
```cpp
vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
```

**매개변수:**
- `fenceCount`: 대기할 Fence 개수
- `waitAll`: `VK_TRUE`면 모든 Fence 대기, `VK_FALSE`면 하나만
- `timeout`: 대기 시간 (나노초), `UINT64_MAX`는 무한 대기

---

### `vkResetFences`
Fence를 unsignaled 상태로 리셋합니다.

**사용 예시:**
```cpp
vkResetFences(device, 1, &fence);
```

---

## 11. Drawing & Presentation

### `vkQueueSubmit`
Command Buffer를 Queue에 제출하여 실행합니다.

**사용 예시:**
```cpp
VkSubmitInfo submitInfo{};
submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
submitInfo.waitSemaphoreCount = 1;
submitInfo.pWaitSemaphores = waitSemaphores;
submitInfo.pWaitDstStageMask = waitStages;

submitInfo.commandBufferCount = 1;
submitInfo.pCommandBuffers = &commandBuffer;

VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
submitInfo.signalSemaphoreCount = 1;
submitInfo.pSignalSemaphores = signalSemaphores;

vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence);
```

**주요 멤버:**
- `waitSemaphores`: 대기할 세마포어
- `waitDstStageMask`: 어느 파이프라인 단계에서 대기할지
- `commandBuffers`: 실행할 커맨드 버퍼
- `signalSemaphores`: 완료 시 시그널할 세마포어

---

### `vkQueuePresentKHR`
렌더링된 이미지를 화면에 표시합니다.

**사용 예시:**
```cpp
VkPresentInfoKHR presentInfo{};
presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
presentInfo.waitSemaphoreCount = 1;
presentInfo.pWaitSemaphores = signalSemaphores;

VkSwapchainKHR swapChains[] = {swapChain};
presentInfo.swapchainCount = 1;
presentInfo.pSwapchains = swapChains;
presentInfo.pImageIndices = &imageIndex;

VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);
```

**반환값:**
- `VK_SUCCESS`: 성공
- `VK_ERROR_OUT_OF_DATE_KHR`: Swapchain 재생성 필요
- `VK_SUBOPTIMAL_KHR`: 표시 가능하지만 최적이 아님

---

## 렌더링 플로우

### 전체 렌더링 과정

```
1. [초기화]
   vkCreateInstance
   → vkEnumeratePhysicalDevices (GPU 선택)
   → vkCreateDevice (논리 디바이스 생성)
   → vkCreateSwapchainKHR (스왑체인 생성)
   → vkCreateImageView (이미지 뷰 생성)
   → vkCreateRenderPass
   → vkCreateGraphicsPipeline
   → vkCreateFramebuffer
   → vkCreateCommandPool
   → vkAllocateCommandBuffers
   → vkCreateSemaphore / vkCreateFence (동기화 객체)

2. [프레임 렌더링 루프]
   vkWaitForFences (이전 프레임 완료 대기)
   → vkAcquireNextImageKHR (다음 이미지 획득)
   → vkResetFences
   → vkResetCommandBuffer
   → vkBeginCommandBuffer
      → vkCmdBeginRenderPass
      → vkCmdBindPipeline
      → vkCmdSetViewport
      → vkCmdSetScissor
      → vkCmdDraw
      → vkCmdEndRenderPass
   → vkEndCommandBuffer
   → vkQueueSubmit (커맨드 버퍼 제출)
   → vkQueuePresentKHR (화면 표시)

3. [정리]
   vkDeviceWaitIdle
   → vkDestroySemaphore
   → vkDestroyFence
   → vkDestroyCommandPool
   → vkDestroyFramebuffer
   → vkDestroyPipeline
   → vkDestroyPipelineLayout
   → vkDestroyRenderPass
   → vkDestroyImageView
   → vkDestroySwapchainKHR
   → vkDestroyDevice
   → vkDestroyInstance
```

---

## 주요 구조체

### VkApplicationInfo
애플리케이션 메타데이터
```cpp
VkApplicationInfo appInfo{};
appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
appInfo.pApplicationName = "My App";
appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
appInfo.pEngineName = "No Engine";
appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
appInfo.apiVersion = VK_API_VERSION_1_0;
```

### VkExtent2D
2D 크기 (너비, 높이)
```cpp
VkExtent2D extent = {800, 600};
```

### VkOffset2D
2D 오프셋 (x, y)
```cpp
VkOffset2D offset = {0, 0};
```

### VkRect2D
2D 사각형 영역
```cpp
VkRect2D rect{};
rect.offset = {0, 0};
rect.extent = {800, 600};
```

### VkViewport
뷰포트 영역
```cpp
VkViewport viewport{};
viewport.x = 0.0f;
viewport.y = 0.0f;
viewport.width = 800.0f;
viewport.height = 600.0f;
viewport.minDepth = 0.0f;
viewport.maxDepth = 1.0f;
```

### VkClearValue
클리어 값 (색상, depth, stencil)
```cpp
VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
```

---

## 베스트 프랙티스

### 1. 동기화
- **Semaphore**: GPU 내부 동기화 (Queue 간, 파이프라인 단계 간)
- **Fence**: CPU-GPU 동기화 (프레임 완료 확인)
- 스왑체인 이미지마다 별도의 세마포어 사용 권장

### 2. 리소스 관리
- 생성 순서의 역순으로 파괴
- `vkDeviceWaitIdle`로 모든 작업 완료 후 리소스 정리
- Command Pool 파괴 시 모든 Command Buffer 자동 해제

### 3. 성능 최적화
- Dynamic State 활용 (Viewport, Scissor)
- 프레임 병렬 처리 (`MAX_FRAMES_IN_FLIGHT`)
- 커맨드 버퍼 재사용 (리셋 후 재기록)

### 4. 오류 처리
- 모든 Vulkan 함수의 반환값 확인
- Validation Layer 활성화 (디버그 빌드)
- Swapchain 재생성 처리 (`VK_ERROR_OUT_OF_DATE_KHR`)

---

## 참고 자료

- [Vulkan Specification](https://www.khronos.org/registry/vulkan/specs/1.3/html/)
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Guide](https://github.com/KhronosGroup/Vulkan-Guide)

---

**작성일:** 2026-01-25  
**프로젝트:** vulkanRenderer
