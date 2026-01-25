#!/bin/bash
# Shader compilation script for Mac/Linux

# Check if glslc is available
if ! command -v glslc &> /dev/null
then
    echo "Error: glslc not found. Please install Vulkan SDK."
    echo "Visit: https://vulkan.lunarg.com/sdk/home"
    echo "Or install via Homebrew: brew install molten-vk"
    exit 1
fi

echo "Compiling shaders..."

# Compile vertex shader
glslc shaders/triangle.vert -o shaders/triangle.vert.spv
if [ $? -eq 0 ]; then
    echo "✓ Compiled triangle.vert -> triangle.vert.spv"
else
    echo "✗ Failed to compile triangle.vert"
    exit 1
fi

# Compile fragment shader
glslc shaders/triangle.frag -o shaders/triangle.frag.spv
if [ $? -eq 0 ]; then
    echo "✓ Compiled triangle.frag -> triangle.frag.spv"
else
    echo "✗ Failed to compile triangle.frag"
    exit 1
fi

echo "All shaders compiled successfully!"
