# Plan: Minimal Initial Architecture Setup

현재 "hello triangle" 상태의 프로젝트를 아키텍처 문서 기반으로 확장 가능한 구조로 재설계합니다. 핵심 Vulkan 추상화 레이어와 기본 렌더링 인프라를 구축하여 향후 Deferred Rendering, SPH/PBD 시뮬레이션 구현의 토대를 마련합니다.

## Steps

1. **디렉토리 구조 재편성**: `src/`를 `core/`, `vulkan/`, `resource/`, `renderer/` 하위 폴더로 분리하고, 기존 파일들(`device`, `swapchain`, `pipeline`, `window`)을 적절한 위치로 이동. CMakeLists.txt 업데이트 필요.

2. **Vulkan Buffer 추상화 추가**: `device.cpp`의 `createBuffer()` 기반으로 `vulkan/buffer.h/cpp` 생성 — VMA 스타일 래퍼로 vertex/index/uniform/storage 버퍼 지원, map/unmap/flush 기능 포함.

3. **Descriptor 관리 시스템 구현**: `vulkan/descriptor.h/cpp`에 `DescriptorPool`, `DescriptorSetLayout`, `DescriptorWriter` 클래스 구현 — uniform buffer와 sampler 바인딩을 위한 빌더 패턴 적용.

4. **Core 시스템 구현**: `core/camera.h/cpp` (Orbit/FPS 카메라, view/projection 행렬), `core/input.h/cpp` (GLFW 키보드/마우스 상태 래핑), `core/timer.h/cpp` (deltaTime, fixedTimestep) 추가.

5. **기본 렌더러 구조 생성**: `renderer/renderer.h/cpp`에 프레임 시작/종료 관리, `vulkan/image.h/cpp`에 텍스처/렌더 타겟 래퍼 추가, `swapchain.cpp` 렌더패스에 depth attachment 추가.

6. **Mesh 시스템 및 3D 오브젝트 테스트**: `resource/mesh.h/cpp`에 `Vertex` 구조체와 vertex/index 버퍼 업로드 기능, push constants 또는 UBO로 MVP 변환 적용하여 회전하는 큐브 렌더링 검증.

## Further Considerations

1. **VMA(Vulkan Memory Allocator) 도입 시점**: 지금 바로 통합
2. **ImGui 통합 우선순위**: 파라미터 디버깅을 위해 초기에 활성화
3. **Compute Pipeline 준비**: 시뮬레이션을 위해 `Pipeline` 클래스를 graphics/compute 모두 지원하도록 확장 필요 — 이 단계에서 분리 설계할지?
