# Vulkan Renderer

Vulkan API를 학습하고 다양한 렌더링 기법을 실험하기 위한 연습 프로젝트입니다.

## 📝 프로젝트 목표

이 프로젝트는 Vulkan의 기초부터 시작하여, 점진적으로 다양한 렌더링 기법을 적용하고 실험하는 것을 목표로 합니다.

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

- [ ] **Phase 3: 고급 렌더링 기법**
  - [ ] Depth Testing
  - [ ] Lighting (Phong/PBR)
  - [ ] Shadow Mapping
  - [ ] Normal Mapping
  - [ ] Post-processing Effects

- [ ] **Phase 4: 최적화 기법**
  - [ ] Frustum Culling
  - [ ] Instancing
  - [ ] Compute Shader
  - [ ] Multi-threading

## 🛠️ 기술 스택

- **API**: Vulkan 1.4.335.0
- **윈도우 관리**: GLFW 3.x
- **수학 라이브러리**: GLM
- **빌드 시스템**: CMake 3.16+
- **컴파일러**: MSVC 2022 (C++17)

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
│   └── glm/                  # GLM 수학 라이브러리 (submodule)
├── docx/
│   └── vulkanApi.md          # Vulkan API 레퍼런스
├── build/                    # 빌드 출력 디렉토리
├── .gitmodules               # Git submodule 설정
├── CMakeLists.txt
├── compile_shaders.bat       # 셰이더 컴파일 스크립트
└── README.md
```

## 🚀 빌드 및 실행

### 사전 요구사항

1. **Vulkan SDK** (1.4.335.0 이상)
   - 설치 경로: `C:\VulkanSDK\1.4.335.0`
   - [다운로드](https://vulkan.lunarg.com/)

2. **CMake** (3.16 이상)
   - [다운로드](https://cmake.org/download/)

3. **Visual Studio 2022**
   - C++ Desktop Development 워크로드 설치

### 빌드 방법

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

### Release 빌드

```powershell
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
.\build\Release\VulkanRenderer.exe
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

- [Vulkan API Reference](docx/vulkanApi.md) - 프로젝트에 사용된 API 정리
- [Vulkan Tutorial](https://vulkan-tutorial.com/) - 공식 튜토리얼
- [Vulkan Specification](https://www.khronos.org/registry/vulkan/)

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

**문제**: 셰이더 컴파일 오류
- **해결**: `compile_shaders.bat` 실행하여 `.spv` 파일 생성

**문제**: Validation Layer 경고
- **해결**: [docx/vulkanApi.md](docx/vulkanApi.md)의 베스트 프랙티스 참고

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
2. `compile_shaders.bat` 실행
3. 프로그램 재실행

## 📄 라이선스

이 프로젝트는 학습 목적의 개인 프로젝트입니다.

## 🙏 참고 자료

- [Vulkan Tutorial by Alexander Overvoorde](https://vulkan-tutorial.com/)
- [Vulkan Guide](https://github.com/KhronosGroup/Vulkan-Guide)

---

**작성일**: 2026-01  
**Vulkan 버전**: 1.4.335.0  
**개발 환경**: Windows 11, Visual Studio 2022
