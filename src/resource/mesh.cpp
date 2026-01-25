#include "mesh.h"

#include <cstring>
#include <unordered_map>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace vkr {

std::vector<VkVertexInputBindingDescription> Mesh::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Mesh::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
    attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
    attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
    attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

    return attributeDescriptions;
}

Mesh::Mesh(Device& device, const Builder& builder) : device_{device} {
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

Mesh::~Mesh() {}

std::unique_ptr<Mesh> Mesh::createFromFile(Device& device, const std::string& filepath) {
    Builder builder{};
    builder.loadFromObjFile(filepath);
    return std::make_unique<Mesh>(device, builder);
}

std::unique_ptr<Mesh> Mesh::createCube(Device& device, glm::vec3 size) {
    Builder builder{};
    
    float hx = size.x * 0.5f;
    float hy = size.y * 0.5f;
    float hz = size.z * 0.5f;
    
    // Define cube vertices with proper normals for each face
    builder.vertices = {
        // Front face (z+) - Red
        {{-hx, -hy, hz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{hx, -hy, hz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{hx, hy, hz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-hx, hy, hz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        
        // Back face (z-) - Cyan
        {{hx, -hy, -hz}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-hx, -hy, -hz}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-hx, hy, -hz}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{hx, hy, -hz}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        
        // Top face (y+) - Yellow
        {{-hx, hy, hz}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{hx, hy, hz}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{hx, hy, -hz}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-hx, hy, -hz}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        
        // Bottom face (y-) - Magenta
        {{-hx, -hy, -hz}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{hx, -hy, -hz}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{hx, -hy, hz}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-hx, -hy, hz}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        
        // Right face (x+) - Green
        {{hx, -hy, hz}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{hx, -hy, -hz}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{hx, hy, -hz}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{hx, hy, hz}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        
        // Left face (x-) - Blue
        {{-hx, -hy, -hz}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-hx, -hy, hz}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-hx, hy, hz}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-hx, hy, -hz}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    };
    
    builder.indices = {
        0, 1, 2, 2, 3, 0,       // Front
        4, 5, 6, 6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,    // Top
        12, 13, 14, 14, 15, 12, // Bottom
        16, 17, 18, 18, 19, 16, // Right
        20, 21, 22, 22, 23, 20  // Left
    };
    
    return std::make_unique<Mesh>(device, builder);
}

std::unique_ptr<Mesh> Mesh::createQuad(Device& device, float width, float height) {
    Builder builder{};
    
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    
    builder.vertices = {
        {{-hw, -hh, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{hw, -hh, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{hw, hh, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-hw, hh, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    };
    
    builder.indices = {0, 1, 2, 2, 3, 0};
    
    return std::make_unique<Mesh>(device, builder);
}

std::unique_ptr<Mesh> Mesh::createSphere(Device& device, float radius, uint32_t segments) {
    Builder builder{};
    
    uint32_t stacks = segments / 2;
    
    for (uint32_t i = 0; i <= stacks; i++) {
        float phi = static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(stacks);
        float y = radius * std::cos(phi);
        float r = radius * std::sin(phi);
        
        for (uint32_t j = 0; j <= segments; j++) {
            float theta = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / static_cast<float>(segments);
            float x = r * std::cos(theta);
            float z = r * std::sin(theta);
            
            glm::vec3 pos = {x, y, z};
            glm::vec3 normal = glm::normalize(pos);
            glm::vec2 uv = {
                static_cast<float>(j) / static_cast<float>(segments),
                static_cast<float>(i) / static_cast<float>(stacks)
            };
            
            builder.vertices.push_back({pos, {1.0f, 1.0f, 1.0f}, normal, uv});
        }
    }
    
    for (uint32_t i = 0; i < stacks; i++) {
        for (uint32_t j = 0; j < segments; j++) {
            uint32_t first = i * (segments + 1) + j;
            uint32_t second = first + segments + 1;
            
            builder.indices.push_back(first);
            builder.indices.push_back(second);
            builder.indices.push_back(first + 1);
            
            builder.indices.push_back(second);
            builder.indices.push_back(second + 1);
            builder.indices.push_back(first + 1);
        }
    }
    
    return std::make_unique<Mesh>(device, builder);
}

std::unique_ptr<Mesh> Mesh::createPlane(Device& device, float width, float depth, uint32_t resX, uint32_t resZ) {
    Builder builder{};
    
    float hw = width * 0.5f;
    float hd = depth * 0.5f;
    
    for (uint32_t z = 0; z <= resZ; z++) {
        for (uint32_t x = 0; x <= resX; x++) {
            float u = static_cast<float>(x) / static_cast<float>(resX);
            float v = static_cast<float>(z) / static_cast<float>(resZ);
            
            glm::vec3 pos = {
                -hw + u * width,
                0.0f,
                -hd + v * depth
            };
            
            builder.vertices.push_back({pos, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {u, v}});
        }
    }
    
    for (uint32_t z = 0; z < resZ; z++) {
        for (uint32_t x = 0; x < resX; x++) {
            uint32_t topLeft = z * (resX + 1) + x;
            uint32_t topRight = topLeft + 1;
            uint32_t bottomLeft = topLeft + resX + 1;
            uint32_t bottomRight = bottomLeft + 1;
            
            builder.indices.push_back(topLeft);
            builder.indices.push_back(bottomLeft);
            builder.indices.push_back(topRight);
            
            builder.indices.push_back(topRight);
            builder.indices.push_back(bottomLeft);
            builder.indices.push_back(bottomRight);
        }
    }
    
    return std::make_unique<Mesh>(device, builder);
}

void Mesh::createVertexBuffers(const std::vector<Vertex>& vertices) {
    vertexCount_ = static_cast<uint32_t>(vertices.size());
    assert(vertexCount_ >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount_;
    uint32_t vertexSize = sizeof(vertices[0]);

    Buffer stagingBuffer{
        device_,
        vertexSize,
        vertexCount_,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)vertices.data());

    vertexBuffer_ = std::make_unique<Buffer>(
        device_,
        vertexSize,
        vertexCount_,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    device_.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer_->getBuffer(), bufferSize);
}

void Mesh::createIndexBuffers(const std::vector<uint32_t>& indices) {
    indexCount_ = static_cast<uint32_t>(indices.size());
    hasIndexBuffer_ = indexCount_ > 0;

    if (!hasIndexBuffer_) {
        return;
    }

    VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount_;
    uint32_t indexSize = sizeof(indices[0]);

    Buffer stagingBuffer{
        device_,
        indexSize,
        indexCount_,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)indices.data());

    indexBuffer_ = std::make_unique<Buffer>(
        device_,
        indexSize,
        indexCount_,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    device_.copyBuffer(stagingBuffer.getBuffer(), indexBuffer_->getBuffer(), bufferSize);
}

void Mesh::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {vertexBuffer_->getBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    if (hasIndexBuffer_) {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer_->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

void Mesh::draw(VkCommandBuffer commandBuffer) {
    if (hasIndexBuffer_) {
        vkCmdDrawIndexed(commandBuffer, indexCount_, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, vertexCount_, 1, 0, 0);
    }
}

void Mesh::Builder::loadFromObjFile(const std::string& filepath) {
    // Minimal OBJ loader - for now just create a placeholder
    // Full implementation would use tinyobjloader or similar
    vertices.clear();
    indices.clear();
}

} // namespace vkr
