cmake_minimum_required(VERSION 3.16)
project(VulkanRenderer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 플랫폼별 Vulkan SDK 경로 설정
if(APPLE)
    # Mac: Homebrew로 설치한 경우 자동 탐지
    # 또는 수동 설치: /usr/local 또는 ~/VulkanSDK
    set(VULKAN_SDK_PATH $ENV{VULKAN_SDK})
else()
    # Windows: 기본 설치 경로
    set(VULKAN_SDK_PATH "C:/VulkanSDK/1.4.335.0")
endif()

# Vulkan 찾기
find_package(Vulkan REQUIRED)

# 프로젝트 경로 설정
set(EXTERN_DIR ${CMAKE_SOURCE_DIR}/extern)

# GLFW 빌드 옵션 설정 (불필요한 빌드 비활성화)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# GLFW submodule 추가
add_subdirectory(${EXTERN_DIR}/glfw)

# GLM은 header-only 라이브러리
set(GLM_DIR ${EXTERN_DIR}/glm)

# 소스 파일
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/window.cpp
    src/device.cpp
    src/swapchain.cpp
    src/pipeline.cpp
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# Include 디렉토리
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${EXTERN_DIR}/glfw/include
    ${GLM_DIR}
)

# 라이브러리 링크
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Vulkan_LIBRARIES}
    glfw
)

# 플랫폼별 설정
if(APPLE)
    # Mac 전용 설정
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        VK_USE_PLATFORM_MACOS_MVK
        VK_USE_PLATFORM_METAL_EXT
        GLFW_INCLUDE_VULKAN
    )
    
    # MoltenVK 프레임워크 링크 (Vulkan SDK에 포함)
    find_library(MOLTEN_VK MoltenVK PATHS
        $ENV{VULKAN_SDK}/lib
        /usr/local/lib
        ~/VulkanSDK/macOS/lib
    )
    if(MOLTEN_VK)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${MOLTEN_VK})
    endif()
    
    # macOS 프레임워크
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
    
    # C++17 표준 설정
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    
elseif(WIN32)
    # Windows 전용 설정
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        GLFW_INCLUDE_VULKAN
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
    
    # UTF-8 소스 코드 인코딩 설정
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    endif()
    
    # MSVC 링크 옵션 (LIBCMT 충돌 해결)
    if(MSVC)
        target_link_options(${PROJECT_NAME} PRIVATE /NODEFAULTLIB:LIBCMT)
    endif()
endif()

# 셰이더 파일 복사 (빌드 후)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMENT "Copying shaders to output directory"
)

# Debug/Release 설정
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_VALIDATION_LAYERS)
endif()
