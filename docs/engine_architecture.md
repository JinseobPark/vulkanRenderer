# Vulkan Fluid & Cloth Simulation Renderer

## 목표
- **Fluid Simulation**: SPH (Smoothed Particle Hydrodynamics) 기반 유체 시뮬레이션
- **Cloth Simulation**: PBD (Position Based Dynamics) 기반 천 시뮬레이션
- **Rendering 고도화**: Deferred Rendering, Post Processing Pipeline

---

## 전체 아키텍처 개요

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           Demo Application                                   │
│  ┌──────────────────────────────────────────────────────────────────────┐   │
│  │  Camera Controller  │  Scene Setup  │  UI (ImGui)  │  Demo Scenes   │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────────────────────┤
│                            Simulation Layer                                  │
│  ┌────────────────────────────────┐  ┌────────────────────────────────────┐ │
│  │       Fluid System             │  │         Cloth System               │ │
│  │  ┌──────────┐ ┌─────────────┐  │  │  ┌──────────┐ ┌────────────────┐   │ │
│  │  │   SPH    │ │  Neighbor   │  │  │  │   PBD    │ │   Constraint   │   │ │
│  │  │  Solver  │ │   Search    │  │  │  │  Solver  │ │     Solver     │   │ │
│  │  └──────────┘ └─────────────┘  │  │  └──────────┘ └────────────────┘   │ │
│  │  ┌────────────────────────┐    │  │  ┌──────────────────────────────┐  │ │
│  │  │  Screen-Space Render   │    │  │  │      Cloth Mesh Render       │  │ │
│  │  └────────────────────────┘    │  │  └──────────────────────────────┘  │ │
│  └────────────────────────────────┘  └────────────────────────────────────┘ │
├─────────────────────────────────────────────────────────────────────────────┤
│                             Rendering Layer                                  │
│  ┌───────────────────────────────────────────────────────────────────────┐  │
│  │                     Deferred Rendering Pipeline                        │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌──────────────┐  │  │
│  │  │ G-Buffer    │  │  Lighting   │  │   Shadow    │  │ Post Process │  │  │
│  │  │   Pass      │──│    Pass     │──│    Pass     │──│    Stack     │  │  │
│  │  └─────────────┘  └─────────────┘  └─────────────┘  └──────────────┘  │  │
│  └───────────────────────────────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────────────────────────────┤
│                              Core Layer                                      │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────────┐ │
│  │Resource Mgr  │  │ Input System │  │ Time Manager │  │   Camera         │ │
│  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────────┘ │
├─────────────────────────────────────────────────────────────────────────────┤
│                            Vulkan Abstraction Layer                          │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────────────┐   │
│  │ Device   │ │ Swapchain│ │ Pipeline │ │ Commands │ │ Memory Allocator │   │
│  └──────────┘ └──────────┘ └──────────┘ └──────────┘ └──────────────────┘   │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 디렉토리 구조

```
src/
├── core/
│   ├── app.h/cpp                 # 메인 애플리케이션 클래스
│   ├── window.h/cpp              # 윈도우 관리 (GLFW)
│   ├── input.h/cpp               # 입력 처리
│   ├── camera.h/cpp              # 카메라 (Orbit/FPS)
│   └── timer.h/cpp               # 프레임 타이밍
│
├── vulkan/
│   ├── device.h/cpp              # Vulkan 디바이스 관리
│   ├── swapchain.h/cpp           # 스왑체인 관리
│   ├── pipeline.h/cpp            # 그래픽/컴퓨트 파이프라인
│   ├── buffer.h/cpp              # GPU 버퍼 추상화
│   ├── image.h/cpp               # GPU 이미지/텍스처
│   ├── descriptor.h/cpp          # 디스크립터 세트 관리
│   ├── command.h/cpp             # 커맨드 풀/버퍼
│   └── sync.h/cpp                # 동기화 객체 (Fence, Semaphore)
│
├── resource/
│   ├── resource_manager.h/cpp    # 리소스 매니저
│   ├── mesh.h/cpp                # 메시 로딩/관리
│   ├── texture.h/cpp             # 텍스처 로딩/관리
│   └── shader.h/cpp              # 셰이더 로딩/컴파일
│
├── renderer/
│   ├── renderer.h/cpp            # 메인 렌더러
│   ├── render_pass.h/cpp         # 렌더 패스 추상화
│   │
│   ├── deferred/
│   │   ├── gbuffer.h/cpp         # G-Buffer 관리
│   │   ├── geometry_pass.h/cpp   # Geometry Pass
│   │   ├── lighting_pass.h/cpp   # Lighting Pass
│   │   └── shadow_pass.h/cpp     # Shadow Mapping
│   │
│   └── post_process/
│       ├── post_process_stack.h/cpp
│       ├── bloom.h/cpp
│       ├── tone_mapping.h/cpp
│       ├── fxaa.h/cpp
│       └── ssao.h/cpp
│
├── simulation/
│   ├── compute_pipeline.h/cpp    # Compute Shader 관리
│   │
│   ├── fluid/
│   │   ├── fluid_system.h/cpp    # 유체 시뮬레이션 메인
│   │   ├── sph_solver.h/cpp      # SPH 알고리즘 (Compute)
│   │   ├── particle.h            # 파티클 데이터 구조
│   │   ├── neighbor_search.h/cpp # Spatial Hash 이웃 탐색
│   │   └── fluid_renderer.h/cpp  # Screen-Space 유체 렌더링
│   │
│   └── cloth/
│       ├── cloth_system.h/cpp    # 천 시뮬레이션 메인
│       ├── pbd_solver.h/cpp      # PBD 알고리즘 (Compute)
│       ├── constraint.h/cpp      # Distance/Bending 제약
│       └── cloth_mesh.h/cpp      # 천 메시 및 렌더링
│
├── scene/
│   ├── scene.h/cpp               # 씬 데이터 (오브젝트 목록)
│   ├── render_object.h/cpp       # 렌더링 가능 오브젝트
│   ├── light.h/cpp               # 라이트 (Directional/Point)
│   └── transform.h/cpp           # Transform 유틸리티
│
├── demo/
│   ├── demo_fluid.h/cpp          # 유체 시뮬레이션 데모
│   ├── demo_cloth.h/cpp          # 천 시뮬레이션 데모
│   └── demo_combined.h/cpp       # 통합 데모
│
└── main.cpp

shaders/
├── deferred/
│   ├── gbuffer.vert/frag
│   ├── lighting.vert/frag
│   ├── shadow.vert/frag
│   └── skybox.vert/frag
│
├── post_process/
│   ├── fullscreen.vert
│   ├── bloom.frag
│   ├── tone_mapping.frag
│   ├── fxaa.frag
│   └── ssao.frag
│
├── fluid/
│   ├── sph_density.comp          # 밀도 계산
│   ├── sph_force.comp            # 힘 계산
│   ├── sph_integrate.comp        # 적분
│   ├── neighbor_hash.comp        # Spatial Hash
│   ├── fluid_depth.vert/frag     # 깊이 렌더링
│   ├── fluid_smooth.frag         # 깊이 스무딩
│   ├── fluid_normal.frag         # 노멀 재구성
│   └── fluid_composite.frag      # 최종 합성
│
└── cloth/
    ├── pbd_predict.comp          # 위치 예측
    ├── pbd_distance.comp         # 거리 제약
    ├── pbd_bending.comp          # 굽힘 제약
    ├── pbd_update.comp           # 최종 업데이트
    └── cloth_render.vert/frag    # 천 렌더링

assets/
├── models/                       # 테스트용 3D 모델
├── textures/                     # 텍스처
└── hdri/                         # 환경맵
```

---

## Application Layer (Demo)

### 애플리케이션 구조

시뮬레이션 데모를 위한 간단한 애플리케이션 레이어입니다.

```cpp
// ===== core/app.h =====

class App {
public:
    void init();
    void run();
    void shutdown();
    
private:
    void update(float deltaTime);
    void render();
    void onResize(int width, int height);
    
    Window window;
    Device device;
    Swapchain swapchain;
    Renderer renderer;
    ResourceManager resources;
    
    Camera camera;
    InputState input;
    Timer timer;
    
    // 현재 데모
    std::unique_ptr<IDemo> currentDemo;
};
```

### 데모 인터페이스

```cpp
// ===== demo/demo_base.h =====

class IDemo {
public:
    virtual ~IDemo() = default;
    
    virtual void init(Renderer& renderer, ResourceManager& resources) = 0;
    virtual void shutdown() = 0;
    
    virtual void update(float deltaTime, const InputState& input) = 0;
    virtual void simulate(float fixedDeltaTime) = 0;
    virtual void render(VkCommandBuffer cmd) = 0;
    
    virtual void onUI() {}  // ImGui 파라미터 조절
    virtual const char* getName() const = 0;
};
```

### 유체 데모
```cpp
// ===== demo/demo_fluid.h =====

class FluidDemo : public IDemo {
public:
    void init(Renderer& renderer, ResourceManager& resources) override {
        fluidSystem.init(device, MAX_PARTICLES);
        fluidRenderer.init(device, renderer.getGBuffer());
        
        // 초기 파티클 배치
        resetParticles();
    }
    
    void update(float deltaTime, const InputState& input) override {
        // 카메라 업데이트, 파라미터 조절 등
    }
    
    void simulate(float fixedDeltaTime) override {
        fluidSystem.step(fixedDeltaTime);
    }
    
    void render(VkCommandBuffer cmd) override {
        fluidRenderer.render(cmd, fluidSystem.getParticleBuffer(),
                            camera.getView(), camera.getProjection());
    }
    
    void onUI() override {
        ImGui::SliderFloat("Viscosity", &params.viscosity, 0.0f, 0.5f);
        ImGui::SliderFloat("Rest Density", &params.restDensity, 500.0f, 2000.0f);
        // ...
    }
    
    const char* getName() const override { return "Fluid Simulation"; }
    
private:
    FluidSystem fluidSystem;
    FluidRenderer fluidRenderer;
    SPHParameters params;
    
    static constexpr uint32_t MAX_PARTICLES = 100000;
    
    void resetParticles();
};
```

### 메인 루프
```cpp
// ===== main.cpp =====

int main() {
    App app;
    app.init();
    app.run();
    app.shutdown();
    return 0;
}

// ===== core/app.cpp =====

void App::run() {
    const float FIXED_TIMESTEP = 1.0f / 120.0f;  // 시뮬레이션 120Hz
    float accumulator = 0.0f;
    
    while (!window.shouldClose()) {
        float deltaTime = timer.tick();
        accumulator += deltaTime;
        
        // Input
        window.pollEvents();
        input.update(window);
        camera.update(deltaTime, input);
        
        // Fixed timestep simulation
        while (accumulator >= FIXED_TIMESTEP) {
            currentDemo->simulate(FIXED_TIMESTEP);
            accumulator -= FIXED_TIMESTEP;
        }
        
        // Update
        currentDemo->update(deltaTime, input);
        
        // Render
        if (auto cmd = renderer.beginFrame()) {
            currentDemo->render(cmd);
            renderer.endFrame();
        }
        
        // UI
        renderUI();
    }
    
    device.waitIdle();
}
```

---

## 리소스 관리 (Resource Management)

시뮬레이션 렌더러에 필요한 간단한 리소스 관리 시스템입니다.

### 리소스 매니저
```cpp
// ===== resource/resource_manager.h =====

class ResourceManager {
public:
    void init(Device& device);
    void shutdown();
    
    // 메시
    Mesh* loadMesh(const std::string& path);
    Mesh* createPlaneMesh(float width, float height, uint32_t resX, uint32_t resY);
    Mesh* createSphereMesh(float radius, uint32_t segments);
    
    // 텍스처
    Texture* loadTexture(const std::string& path);
    Texture* loadHDR(const std::string& path);
    Texture* createSolidColor(glm::vec4 color);
    
    // 셰이더
    VkShaderModule loadShader(const std::string& spvPath);
    
private:
    Device* device;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    std::unordered_map<std::string, VkShaderModule> shaders;
};
```

### 메시
```cpp
// ===== resource/mesh.h =====

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {
public:
    void upload(Device& device, const std::vector<Vertex>& vertices,
                const std::vector<uint32_t>& indices);
    void destroy(Device& device);
    
    void bind(VkCommandBuffer cmd);
    void draw(VkCommandBuffer cmd);
    
    uint32_t getIndexCount() const { return indexCount; }
    
private:
    Buffer vertexBuffer;
    Buffer indexBuffer;
    uint32_t indexCount = 0;
};
```

### 텍스처
```cpp
// ===== resource/texture.h =====

class Texture {
public:
    void create(Device& device, uint32_t width, uint32_t height, 
                VkFormat format, VkImageUsageFlags usage);
    void upload(Device& device, const void* data, size_t size);
    void destroy(Device& device);
    
    VkImageView getView() const { return imageView; }
    VkSampler getSampler() const { return sampler; }
    VkDescriptorImageInfo getDescriptorInfo() const;
    
private:
    VkImage image = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
    uint32_t width, height;
    VkFormat format;
};
```

---

## 핵심 시스템 상세 설계

### 1. Vulkan Abstraction Layer

#### Memory Allocator (VMA 사용 권장)
```cpp
class MemoryAllocator {
public:
    VmaAllocator allocator;
    
    Buffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, 
                        VmaMemoryUsage memoryUsage);
    Image createImage(const ImageCreateInfo& info);
    
    // Staging buffer for CPU → GPU transfer
    void uploadToGPU(Buffer& dst, const void* data, size_t size);
};
```

#### Buffer 추상화
```cpp
class Buffer {
public:
    VkBuffer buffer;
    VmaAllocation allocation;
    VkDeviceSize size;
    
    void* map();
    void unmap();
    void flush();
};
```

### 2. Deferred Rendering Pipeline

#### G-Buffer 구성
| Attachment | Format | Content |
|------------|--------|---------|
| RT0 | RGBA16F | Position (World Space) |
| RT1 | RGBA16F | Normal + Roughness |
| RT2 | RGBA8 | Albedo + Metallic |
| RT3 | RGBA8 | Emissive + AO |
| Depth | D32F | Depth Buffer |

#### Render Pipeline 흐름
```
1. Shadow Pass (Directional/Point/Spot lights)
   └── Shadow Maps 생성

2. G-Buffer Pass
   └── 모든 불투명 오브젝트 렌더링

3. SSAO Pass (Optional)
   └── G-Buffer로부터 AO 계산

4. Lighting Pass
   └── G-Buffer + Shadow Maps → Lit Scene (HDR)

5. Transparent Pass
   └── Forward Rendering for 투명 오브젝트

6. Post Processing Stack
   ├── Bloom
   ├── Motion Blur (Optional)
   ├── DOF (Optional)
   ├── Tone Mapping
   ├── FXAA
   └── Final Composite to Swapchain
```

#### G-Buffer 클래스
```cpp
class GBuffer {
public:
    void create(uint32_t width, uint32_t height);
    void resize(uint32_t width, uint32_t height);
    
    VkRenderPass getRenderPass() const;
    VkFramebuffer getFramebuffer() const;
    
    const Texture& getPosition() const;
    const Texture& getNormal() const;
    const Texture& getAlbedo() const;
    const Texture& getDepth() const;
    
private:
    std::array<Texture, 4> colorAttachments;
    Texture depthAttachment;
    VkRenderPass renderPass;
    VkFramebuffer framebuffer;
};
```

### 3. Post Processing Stack

```cpp
class PostProcessStack {
public:
    void init(Device& device, uint32_t width, uint32_t height);
    
    void addEffect(std::unique_ptr<PostProcessEffect> effect);
    void removeEffect(const std::string& name);
    
    // Ping-pong rendering between HDR buffers
    void execute(VkCommandBuffer cmd, const Texture& input);
    
    const Texture& getOutput() const;
    
private:
    std::vector<std::unique_ptr<PostProcessEffect>> effects;
    std::array<Texture, 2> pingPongBuffers;  // HDR buffers
    int currentBuffer = 0;
};

class PostProcessEffect {
public:
    virtual void execute(VkCommandBuffer cmd, 
                        const Texture& input, 
                        const Texture& output) = 0;
    virtual bool isEnabled() const = 0;
};

class BloomEffect : public PostProcessEffect {
public:
    float threshold = 1.0f;
    float intensity = 1.0f;
    int mipLevels = 5;
    
    void execute(VkCommandBuffer cmd, 
                const Texture& input, 
                const Texture& output) override;
                
private:
    std::vector<Texture> downsampleChain;
    std::vector<Texture> upsampleChain;
    Pipeline downsamplePipeline;
    Pipeline upsamplePipeline;
};
```

### 4. Fluid Simulation (SPH)

#### 파티클 데이터 구조
```cpp
struct FluidParticle {
    glm::vec3 position;
    float density;
    glm::vec3 velocity;
    float pressure;
    glm::vec3 force;
    float padding;
};
```

#### SPH Solver (GPU Compute)
```cpp
class SPHSolver {
public:
    void init(Device& device, uint32_t maxParticles);
    
    void setParameters(const SPHParameters& params);
    void simulate(VkCommandBuffer cmd, float dt);
    
    Buffer& getParticleBuffer();
    uint32_t getParticleCount() const;
    
private:
    Buffer particleBuffer;       // SSBO for particles
    Buffer gridBuffer;           // Spatial hash grid
    Buffer cellStartBuffer;      // Grid cell boundaries
    
    Pipeline densityPipeline;    // 밀도 계산
    Pipeline forcePipeline;      // 압력/점성 힘 계산
    Pipeline integratePipeline;  // 시간 적분
    Pipeline neighborPipeline;   // 이웃 탐색 (정렬 기반)
    
    SPHParameters params;
};

struct SPHParameters {
    float restDensity = 1000.0f;
    float gasConstant = 2000.0f;
    float viscosity = 0.01f;
    float smoothingRadius = 0.1f;
    float particleMass = 0.02f;
    float gravity = -9.81f;
    float boundaryDamping = 0.3f;
};
```

#### Fluid Rendering (Screen Space)
```cpp
class FluidRenderer {
public:
    void render(VkCommandBuffer cmd, 
                const Buffer& particles,
                const glm::mat4& view,
                const glm::mat4& proj);
                
private:
    // 1. Point Sprites → Depth
    // 2. Depth Smoothing (Bilateral Filter)
    // 3. Normal Reconstruction
    // 4. Thickness Rendering
    // 5. Shading (Refraction, Reflection, Fresnel)
    
    Pipeline depthPipeline;
    Pipeline smoothPipeline;
    Pipeline normalPipeline;
    Pipeline thicknessPipeline;
    Pipeline compositePipeline;
    
    Texture depthTexture;
    Texture smoothDepthTexture;
    Texture normalTexture;
    Texture thicknessTexture;
};
```

### 5. Cloth Simulation (PBD)

#### 천 데이터 구조
```cpp
struct ClothVertex {
    glm::vec3 position;
    float invMass;
    glm::vec3 prevPosition;
    float padding;
    glm::vec3 velocity;
    float padding2;
};

struct DistanceConstraint {
    uint32_t particleA;
    uint32_t particleB;
    float restLength;
    float stiffness;
};

struct BendingConstraint {
    uint32_t particles[4];  // Quad vertices
    float restAngle;
    float stiffness;
    float padding[2];
};
```

#### PBD Solver
```cpp
class PBDSolver {
public:
    void init(Device& device);
    
    void setCloth(const std::vector<ClothVertex>& vertices,
                  const std::vector<DistanceConstraint>& distance,
                  const std::vector<BendingConstraint>& bending);
                  
    void simulate(VkCommandBuffer cmd, float dt, int substeps = 4);
    
    Buffer& getVertexBuffer();
    
private:
    Buffer vertexBuffer;
    Buffer distanceConstraintBuffer;
    Buffer bendingConstraintBuffer;
    
    Pipeline predictPipeline;      // 위치 예측
    Pipeline distancePipeline;     // 거리 제약
    Pipeline bendingPipeline;      // 굽힘 제약
    Pipeline updatePipeline;       // 속도/위치 업데이트
    
    uint32_t vertexCount;
    uint32_t constraintCount;
};
```

### 6. Compute Pipeline 관리

```cpp
class ComputeContext {
public:
    void init(Device& device);
    
    VkCommandBuffer beginCompute();
    void endCompute(VkCommandBuffer cmd);
    
    void dispatchPipeline(VkCommandBuffer cmd,
                          const Pipeline& pipeline,
                          uint32_t groupCountX,
                          uint32_t groupCountY = 1,
                          uint32_t groupCountZ = 1);
    
    void barrier(VkCommandBuffer cmd,
                 const Buffer& buffer,
                 VkAccessFlags srcAccess,
                 VkAccessFlags dstAccess);
                 
private:
    VkCommandPool computePool;
    VkQueue computeQueue;
    VkFence computeFence;
};
```

---

## Frame 동기화 전략

### Multi-Frame 동기화
```cpp
static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

class FrameSync {
public:
    struct FrameData {
        VkSemaphore imageAvailable;
        VkSemaphore renderFinished;
        VkSemaphore computeFinished;
        VkFence inFlightFence;
        VkCommandBuffer graphicsCmd;
        VkCommandBuffer computeCmd;
    };
    
    std::array<FrameData, MAX_FRAMES_IN_FLIGHT> frames;
    uint32_t currentFrame = 0;
    
    void nextFrame() { currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT; }
};
```

### Graphics-Compute 동기화
```
Frame N:
┌─────────────────┐     ┌─────────────────┐
│  Compute Queue  │     │ Graphics Queue  │
├─────────────────┤     ├─────────────────┤
│ Simulation      │────▶│ G-Buffer Pass   │
│ (Fluid/Cloth)   │     │ Lighting Pass   │
│                 │     │ Post Process    │
└─────────────────┘     └─────────────────┘
        │                        │
        └── Semaphore ───────────┘
```

---

## 주요 최적화 기법

### 1. 메모리 최적화
- **VMA (Vulkan Memory Allocator)** 사용
- Buffer aliasing: 같은 프레임 내 비중첩 버퍼 공유
- Staging buffer 풀링

### 2. 렌더링 최적화
- **Indirect Drawing**: GPU-driven culling과 결합
- **Bindless Textures**: Descriptor Indexing 사용
- **Visibility Buffer**: 대규모 씬에서 G-Buffer 대안

### 3. 시뮬레이션 최적화
- **Spatial Hashing**: 이웃 탐색 O(n) → O(1)
- **GPU Radix Sort**: 파티클 정렬 (SPH)
- **Jacobi Iteration**: 병렬 제약 해결 (PBD)

### 4. 동기화 최적화
- Compute-Graphics 병렬화 (Async Compute)
- Timeline Semaphores (Vulkan 1.2+)

---

## 확장 가능한 기능 (향후 고려)

1. **Ray Tracing** (VK_KHR_ray_tracing)
   - Reflections, Global Illumination
   
2. **Mesh Shaders** (VK_EXT_mesh_shader)
   - GPU-driven geometry processing
   
3. **Variable Rate Shading** (VK_KHR_fragment_shading_rate)
   - 화면 영역별 셰이딩 율 조절

4. **Advanced Fluid**
   - FLIP/APIC hybrid method
   - Surface reconstruction (Marching Cubes)
   
5. **Hair/Fur Simulation**
   - TressFX 스타일 시뮬레이션

---

## 참고 자료

### Vulkan
- [Vulkan Specification](https://www.khronos.org/registry/vulkan/specs/1.3/html/)
- [Vulkan Guide](https://vkguide.dev/)
- [Vulkan Memory Allocator](https://gpuopen.com/vulkan-memory-allocator/)

### Deferred Rendering
- [Learn OpenGL - Deferred Shading](https://learnopengl.com/Advanced-Lighting/Deferred-Shading)
- [Filament Documentation](https://google.github.io/filament/)

### Fluid Simulation
- [Position Based Fluids (Macklin & Müller)](https://mmacklin.com/pbf_sig_preprint.pdf)
- [Screen Space Fluid Rendering](https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-30-real-time-simulation-and-rendering-3d-fluids)

### Cloth Simulation
- [XPBD (Extended Position Based Dynamics)](https://matthias-research.github.io/pages/publications/XPBD.pdf)
- [GPU Cloth Simulation (NVIDIA)](https://developer.nvidia.com/content/gpu-accelerated-cloth-simulation-physics-x)
