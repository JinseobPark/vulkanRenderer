#pragma once

#include <chrono>

namespace vkr {

class Timer {
public:
    Timer();
    
    // Call at beginning of each frame
    float tick();
    
    // Time accessors
    float getDeltaTime() const { return deltaTime_; }
    float getTotalTime() const { return totalTime_; }
    float getFPS() const { return fps_; }
    uint64_t getFrameCount() const { return frameCount_; }
    
    // Fixed timestep helpers
    float getFixedDeltaTime() const { return fixedDeltaTime_; }
    void setFixedDeltaTime(float dt) { fixedDeltaTime_ = dt; }
    
    // Returns accumulated time for fixed timestep simulation
    float getAccumulator() const { return accumulator_; }
    void consumeAccumulator(float dt) { accumulator_ -= dt; }
    void addToAccumulator(float dt) { accumulator_ += dt; }
    bool shouldSimulate() const { return accumulator_ >= fixedDeltaTime_; }

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    TimePoint startTime_;
    TimePoint lastFrameTime_;
    
    float deltaTime_ = 0.0f;
    float totalTime_ = 0.0f;
    float fixedDeltaTime_ = 1.0f / 120.0f;  // 120Hz for simulation
    float accumulator_ = 0.0f;
    
    // FPS calculation
    float fpsAccumulator_ = 0.0f;
    int fpsFrameCount_ = 0;
    float fps_ = 0.0f;
    
    uint64_t frameCount_ = 0;
};

} // namespace vkr
