# Vulkan Fluid & Cloth Simulation Renderer

Vulkan API를 활용한 고급 물리 시뮬레이션 및 렌더링 프로젝트입니다.

## 📝 프로젝트 목표

이 프로젝트는 Vulkan Compute Shader를 활용한 실시간 물리 시뮬레이션과 고급 렌더링 기법을 구현하는 것을 목표로 합니다.

### 주요 목표

- **Fluid Simulation**: SPH (Smoothed Particle Hydrodynamics) 기반 실시간 유체 시뮬레이션
- **Cloth Simulation**: PBD (Position Based Dynamics) 기반 천 시뮬레이션
- **Deferred Rendering**: G-Buffer 기반 고급 렌더링 파이프라인
- **Post Processing**: Bloom, Tone Mapping, FXAA 등 후처리 효과

### 단계별 목표

- [x] **Phase 1: 기본 설정**
  - [x] Vulkan 초기화 및 인스턴스 생성
  - [x] Physical/Logical Device 설정
  - [x] Swapchain 구성
  - [x] 기본 삼각형 렌더링

- [ ] **Phase 2: 기본 렌더링**
  - [ ] Vertex Buffer & Index Buffer
  - [ ] Uniform Buffer (MVP 행렬)
  - [ ] 텍스처 매핑
  - [ ] 3D 모델 로딩

- [ ] **Phase 3: Deferred Rendering Pipeline**
  - [ ] G-Buffer 구성 (Position, Normal, Albedo)
  - [ ] Lighting Pass (Deferred Shading)
  - [ ] Shadow Mapping
  - [ ] SSAO (Screen Space Ambient Occlusion)

- [ ] **Phase 4: Fluid Simulation (SPH)**
  - [ ] Compute Shader 기반 SPH Solver
  - [ ] Spatial Hashing (이웃 탐색)
  - [ ] Screen-Space Fluid Rendering
  - [ ] Depth Smoothing & Normal Reconstruction

- [ ] **Phase 5: Cloth Simulation (PBD)**
  - [ ] PBD Solver (Compute Shader)
  - [ ] Distance & Bending Constraints
  - [ ] Collision Detection
  - [ ] Cloth Mesh Rendering

- [ ] **Phase 6: Post Processing**
  - [ ] Bloom Effect
  - [ ] Tone Mapping (HDR)
  - [ ] FXAA (Anti-aliasing)
  - [ ] Post Process Stack

## 🛠️ 기술 스택

- **API**: Vulkan 1.4.335.0
- **윈도우 관리**: GLFW 3.x
- **수학 라이브러리**: GLM
- **UI 라이브러리**: ImGui (docking branch)
- **빌드 시스템**: CMake 3.16+
- **컴파일러**: MSVC 2022 (C++17) / Clang (Mac)
- **플랫폼**: Windows, macOS (MoltenVK)

## 📁 프로젝트 구조

```
vulkanRenderer/
├── src/
│   └── main.cpp              # 메인 애플리케이션 코드
├── shaders/
│   ├── triangle.vert         # Vertex 셰이더
│   ├── triangle.frag         # Fragment 셰이더
│   ├── triangle.vert.spv     # 컴파일된 Vertex 셰이더
│   └── triangle.frag.spv     # 컴파일된 Fragment 셰이더
├── extern/
│   ├── glfw/                 # GLFW 라이브러리 (submodule)
│   ├── glm/                  # GLM 수학 라이브러리 (submodule)
│   └── imgui/                # ImGui UI 라이브러리 (submodule)
├── docs/
│   ├── engine_architecture.md # 엔진 아키텍처 설계 문서
│   └── vulkanApi.md          # Vulkan API 레퍼런스
├── build/                    # 빌드 출력 디렉토리
├── .gitmodules               # Git submodule 설정
├── CMakeLists.txt
├── compile_shaders.bat       # 셰이더 컴파일 스크립트
└── README.md
```

## 🚀 빌드 및 실행

### 사전 요구사항

#### Windows

1. **Vulkan SDK** (1.4.335.0 이상)
   - 설치 경로: `C:\VulkanSDK\1.4.335.0`
   - [다운로드](https://vulkan.lunarg.com/)

2. **CMake** (3.16 이상)
   - [다운로드](https://cmake.org/download/)

3. **Visual Studio 2022**
   - C++ Desktop Development 워크로드 설치

#### macOS

1. **Vulkan SDK 및 MoltenVK**
   ```bash
   # Homebrew로 설치 (권장)
   brew install molten-vk vulkan-loader vulkan-headers
   
   # 또는 공식 SDK 다운로드
   # https://vulkan.lunarg.com/sdk/home
   ```

2. **CMake**
   ```bash
   brew install cmake
   ```

3. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```

### 빌드 방법

#### Windows

```powershell
# 1. submodule 초기화 (최초 클론 시)
git submodule update --init --recursive

# 2. 셰이더 컴파일
.\compile_shaders.bat

# 3. CMake 프로젝트 생성
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 4. 빌드 실행
cmake --build build --config Debug

# 5. 실행
.\build\Debug\VulkanRenderer.exe
```

#### macOS

```bash
# 1. submodule 초기화 (최초 클론 시)
git submodule update --init --recursive

# 2. 셰이더 컴파일 스크립트에 실행 권한 부여
chmod +x compile_shaders.sh

# 3. 셰이더 컴파일
./compile_shaders.sh

# 4. CMake 프로젝트 생성
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 5. 빌드 실행
cmake --build build --config Debug

# 6. 실행
./build/Debug/VulkanRenderer
# 또는
./build/VulkanRenderer  # 빌드 시스템에 따라 다를 수 있음
```

### Release 빌드

#### Windows
```powershell
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
.\build\Release\VulkanRenderer.exe
```

#### macOS
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/Release/VulkanRenderer
```

## 🎮 현재 구현된 기능

### ✅ 완료된 기능

- **Vulkan 초기화**
  - Instance 생성 및 Validation Layer 설정
  - Physical Device 선택 (GPU)
  - Logical Device 및 Queue 생성

- **렌더링 파이프라인**
  - Swapchain 구성 (더블 버퍼링)
  - Render Pass 생성
  - Graphics Pipeline (Vertex + Fragment Shader)
  - Framebuffer 생성

- **동기화**
  - Semaphore를 통한 이미지별 동기화
  - Fence를 통한 CPU-GPU 동기화
  - Frame-in-flight 처리

- **기본 렌더링**
  - 컬러 삼각형 렌더링
  - 창 크기 조절 지원 (Swapchain 재생성)

## 📚 학습 자료

### 프로젝트 문서
- [Engine Architecture](docs/engine_architecture.md) - 전체 시스템 아키텍처 및 설계
- [Vulkan API Reference](docs/vulkanApi.md) - 프로젝트에 사용된 API 정리

### Vulkan
- [Vulkan Tutorial](https://vulkan-tutorial.com/) - 공식 튜토리얼
- [Vulkan Specification](https://www.khronos.org/registry/vulkan/)
- [Vulkan Guide](https://github.com/KhronosGroup/Vulkan-Guide)

### Fluid Simulation
- [Position Based Fluids (Macklin & Müller)](https://mmacklin.com/pbf_sig_preprint.pdf)
- [Screen Space Fluid Rendering (NVIDIA GPU Gems 3)](https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-30-real-time-simulation-and-rendering-3d-fluids)

### Cloth Simulation
- [XPBD (Extended Position Based Dynamics)](https://matthias-research.github.io/pages/publications/XPBD.pdf)
- [GPU Cloth Simulation (NVIDIA)](https://developer.nvidia.com/content/gpu-accelerated-cloth-simulation-physics-x)

## 🐛 디버깅

### Validation Layer 활성화

Debug 빌드에서는 Validation Layer가 자동으로 활성화되어 오류를 감지합니다.

```cpp
#ifdef ENABLE_VALIDATION_LAYERS
const bool enableValidationLayers = true;
#endif
```

### 일반적인 문제 해결

**문제**: `Failed to create Vulkan instance`
- **해결**: Vulkan SDK가 올바르게 설치되었는지 확인
- 환경변수 `VULKAN_SDK`가 설정되어 있는지 확인
- (Mac) MoltenVK가 올바르게 설치되었는지 확인: `brew list molten-vk`

**문제**: 셰이더 컴파일 오류
- **해결** (Windows): `compile_shaders.bat` 실행하여 `.spv` 파일 생성
- **해결** (Mac): `./compile_shaders.sh` 실행하여 `.spv` 파일 생성
- `glslc` 명령어가 없는 경우 Vulkan SDK 설치 확인

**문제**: Validation Layer 경고
- **해결**: [docx/vulkanApi.md](docx/vulkanApi.md)의 베스트 프랙티스 참고

**문제** (Mac): `dyld: Library not loaded: @rpath/libvulkan.1.dylib`
- **해결**: Vulkan SDK 환경변수 설정
  ```bash
  export VULKAN_SDK=~/VulkanSDK/x.x.x.x/macOS  # 버전에 맞게 수정
  export PATH=$VULKAN_SDK/bin:$PATH
  export DYLD_LIBRARY_PATH=$VULKAN_SDK/lib:$DYLD_LIBRARY_PATH
  export VK_ICD_FILENAMES=$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json
  export VK_LAYER_PATH=$VULKAN_SDK/share/vulkan/explicit_layer.d
  ```
  - 또는 `~/.zshrc` 또는 `~/.bash_profile`에 추가

## 🔧 개발 환경 설정

### VS Code 추천 확장

- C/C++
- CMake Tools
- Shader languages support for VS Code

### 디렉토리 구조 변경 시

CMake 재구성:
```powershell
cmake -B build
```

### 셰이더 수정 시

1. `.vert` 또는 `.frag` 파일 수정
2. 셰이더 컴파일
   - Windows: `compile_shaders.bat` 실행
   - Mac/Linux: `./compile_shaders.sh` 실행
3. 프로그램 재실행

## ⚠️ 플랫폼별 주의사항

### macOS (MoltenVK)

- **지원 버전**: MoltenVK는 Vulkan 1.2까지 지원하며, 일부 확장 기능은 제한될 수 있습니다.
- **성능**: Vulkan API가 Metal로 변환되는 과정에서 약간의 오버헤드가 발생할 수 있습니다.
- **Validation Layers**: 일부 레이어가 Windows와 다르게 동작하거나 사용 불가능할 수 있습니다.
- **환경변수**: Vulkan SDK가 Homebrew로 설치된 경우, 환경변수 설정이 자동으로 처리됩니다.

### Windows

- **Vulkan SDK**: 네이티브 Vulkan 드라이버가 GPU에서 직접 실행됩니다.
- **Visual Studio**: MSVC 컴파일러를 사용하여 최적화된 빌드가 가능합니다.

## 📄 라이선스

이 프로젝트는 학습 목적의 개인 프로젝트입니다.

## 🙏 참고 자료

- [Vulkan Tutorial by Alexander Overvoorde](https://vulkan-tutorial.com/)
- [Vulkan Guide](https://github.com/KhronosGroup/Vulkan-Guide)
- [GPU Gems 3 - Fluid Simulation](https://developer.nvidia.com/gpugems/gpugems3)
- [Real-Time Rendering Resources](http://www.realtimerendering.com/)

---

**작성일**: 2026-01  
**Vulkan 버전**: 1.4.335.0  
**지원 플랫폼**: Windows 11, macOS (MoltenVK)  
**개발 환경**: Visual Studio 2022 / Xcode
