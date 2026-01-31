#pragma once

#include "../vulkan/device.h"
#include "../vulkan/buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace rs {

class Mesh {
public:
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color &&
                   normal == other.normal && uv == other.uv;
        }
    };

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadFromObjFile(const std::string& filepath);
    };

    Mesh(Device& device, const Builder& builder);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    static std::unique_ptr<Mesh> createFromFile(Device& device, const std::string& filepath);
    
    // Primitive mesh generators
    static std::unique_ptr<Mesh> createCube(Device& device, glm::vec3 size = glm::vec3(1.0f));
    static std::unique_ptr<Mesh> createQuad(Device& device, float width = 1.0f, float height = 1.0f);
    static std::unique_ptr<Mesh> createSphere(Device& device, float radius = 0.5f, uint32_t segments = 32);
    static std::unique_ptr<Mesh> createPlane(Device& device, float width = 10.0f, float depth = 10.0f, uint32_t resX = 10, uint32_t resZ = 10);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

    uint32_t getVertexCount() const { return vertexCount_; }
    uint32_t getIndexCount() const { return indexCount_; }

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

    Device& device_;

    std::unique_ptr<Buffer> vertexBuffer_;
    uint32_t vertexCount_;

    bool hasIndexBuffer_ = false;
    std::unique_ptr<Buffer> indexBuffer_;
    uint32_t indexCount_;
};

} // namespace rs
