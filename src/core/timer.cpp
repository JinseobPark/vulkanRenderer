#include "timer.h"

#include <algorithm>

namespace rs {

Timer::Timer() {
    startTime_ = Clock::now();
    lastFrameTime_ = startTime_;
}

float Timer::tick() {
    TimePoint currentTime = Clock::now();
    
    auto duration = std::chrono::duration<float>(currentTime - lastFrameTime_);
    deltaTime_ = duration.count();
    
    // Clamp delta time to prevent spiral of death
    deltaTime_ = std::min(deltaTime_, 0.25f);
    
    auto totalDuration = std::chrono::duration<float>(currentTime - startTime_);
    totalTime_ = totalDuration.count();
    
    lastFrameTime_ = currentTime;
    frameCount_++;
    
    // Accumulator for fixed timestep
    accumulator_ += deltaTime_;
    
    // FPS calculation (update every second)
    fpsAccumulator_ += deltaTime_;
    fpsFrameCount_++;
    if (fpsAccumulator_ >= 1.0f) {
        fps_ = static_cast<float>(fpsFrameCount_) / fpsAccumulator_;
        fpsAccumulator_ = 0.0f;
        fpsFrameCount_ = 0;
    }
    
    return deltaTime_;
}

} // namespace rs
