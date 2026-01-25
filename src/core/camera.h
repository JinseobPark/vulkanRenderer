#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vkr {

class Camera {
public:
    enum class Mode {
        Orbit,      // Rotates around a target point
        FPS         // First person style
    };

    Camera();
    
    void update(float deltaTime);
    
    // Transform
    void setPosition(const glm::vec3& position) { position_ = position; updateView(); }
    void setTarget(const glm::vec3& target) { target_ = target; updateView(); }
    void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));
    
    // Projection
    void setPerspective(float fov, float aspect, float nearPlane, float farPlane);
    void setAspectRatio(float aspect);
    
    // Mode
    void setMode(Mode mode) { mode_ = mode; }
    Mode getMode() const { return mode_; }
    
    // Camera control (call from input handler)
    void rotate(float deltaYaw, float deltaPitch);
    void zoom(float delta);
    void pan(float deltaX, float deltaY);
    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    
    // Orbit mode settings
    void setOrbitDistance(float distance) { orbitDistance_ = distance; updateOrbitPosition(); }
    float getOrbitDistance() const { return orbitDistance_; }
    void setOrbitSpeed(float speed) { orbitSpeed_ = speed; }
    
    // FPS mode settings
    void setMoveSpeed(float speed) { moveSpeed_ = speed; }
    void setMouseSensitivity(float sensitivity) { mouseSensitivity_ = sensitivity; }
    
    // Getters
    const glm::mat4& getViewMatrix() const { return viewMatrix_; }
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix_; }
    glm::mat4 getViewProjectionMatrix() const { return projectionMatrix_ * viewMatrix_; }
    
    const glm::vec3& getPosition() const { return position_; }
    const glm::vec3& getTarget() const { return target_; }
    glm::vec3 getForward() const { return glm::normalize(target_ - position_); }
    glm::vec3 getRight() const { return glm::normalize(glm::cross(getForward(), up_)); }
    glm::vec3 getUp() const { return up_; }
    
    float getYaw() const { return yaw_; }
    float getPitch() const { return pitch_; }
    float getFov() const { return fov_; }
    float getNear() const { return nearPlane_; }
    float getFar() const { return farPlane_; }

private:
    void updateView();
    void updateProjection();
    void updateOrbitPosition();
    void updateFPSVectors();
    
    Mode mode_ = Mode::Orbit;
    
    // Position & orientation
    glm::vec3 position_{0.0f, 2.0f, 5.0f};
    glm::vec3 target_{0.0f, 0.0f, 0.0f};
    glm::vec3 up_{0.0f, 1.0f, 0.0f};
    
    float yaw_ = -90.0f;    // Degrees
    float pitch_ = 0.0f;    // Degrees
    
    // Orbit mode
    float orbitDistance_ = 5.0f;
    float orbitSpeed_ = 0.3f;
    float zoomSpeed_ = 0.5f;
    float minOrbitDistance_ = 0.5f;
    float maxOrbitDistance_ = 100.0f;
    
    // FPS mode
    float moveSpeed_ = 5.0f;
    float mouseSensitivity_ = 0.1f;
    
    // Pitch limits
    float minPitch_ = -89.0f;
    float maxPitch_ = 89.0f;
    
    // Projection
    float fov_ = 45.0f;
    float aspectRatio_ = 16.0f / 9.0f;
    float nearPlane_ = 0.1f;
    float farPlane_ = 1000.0f;
    
    // Matrices
    glm::mat4 viewMatrix_{1.0f};
    glm::mat4 projectionMatrix_{1.0f};
};

} // namespace vkr
