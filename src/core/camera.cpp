#include "camera.h"

#include <algorithm>
#include <cmath>

namespace rs {

Camera::Camera() {
    updateView();
    updateProjection();
}

void Camera::update(float deltaTime) {
    // Called each frame - can add smooth interpolation here if needed
}

void Camera::lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
    position_ = eye;
    target_ = target;
    up_ = up;
    
    // Calculate orbit distance
    orbitDistance_ = glm::length(target_ - position_);
    
    // Calculate yaw and pitch from direction
    glm::vec3 direction = glm::normalize(target_ - position_);
    pitch_ = glm::degrees(std::asin(direction.y));
    yaw_ = glm::degrees(std::atan2(direction.z, direction.x));
    
    updateView();
}

void Camera::setPerspective(float fov, float aspect, float nearPlane, float farPlane) {
    fov_ = fov;
    aspectRatio_ = aspect;
    nearPlane_ = nearPlane;
    farPlane_ = farPlane;
    updateProjection();
}

void Camera::setAspectRatio(float aspect) {
    aspectRatio_ = aspect;
    updateProjection();
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    if (mode_ == Mode::Orbit) {
        yaw_ += deltaYaw * orbitSpeed_;
        pitch_ += deltaPitch * orbitSpeed_;
    } else {
        yaw_ += deltaYaw * mouseSensitivity_;
        pitch_ += deltaPitch * mouseSensitivity_;
    }
    
    // Clamp pitch
    pitch_ = std::clamp(pitch_, minPitch_, maxPitch_);
    
    if (mode_ == Mode::Orbit) {
        updateOrbitPosition();
    } else {
        updateFPSVectors();
    }
}

void Camera::zoom(float delta) {
    if (mode_ == Mode::Orbit) {
        orbitDistance_ -= delta * zoomSpeed_;
        orbitDistance_ = std::clamp(orbitDistance_, minOrbitDistance_, maxOrbitDistance_);
        updateOrbitPosition();
    } else {
        // In FPS mode, zoom could change FOV or move forward
        moveForward(delta);
    }
}

void Camera::pan(float deltaX, float deltaY) {
    glm::vec3 right = getRight();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    glm::vec3 offset = right * deltaX + up * deltaY;
    
    if (mode_ == Mode::Orbit) {
        target_ += offset * 0.01f * orbitDistance_;
        updateOrbitPosition();
    } else {
        position_ += offset * 0.01f;
        updateView();
    }
}

void Camera::moveForward(float delta) {
    if (mode_ == Mode::FPS) {
        position_ += getForward() * delta * moveSpeed_;
        target_ = position_ + getForward();
        updateView();
    }
}

void Camera::moveRight(float delta) {
    if (mode_ == Mode::FPS) {
        position_ += getRight() * delta * moveSpeed_;
        target_ = position_ + getForward();
        updateView();
    }
}

void Camera::moveUp(float delta) {
    if (mode_ == Mode::FPS) {
        position_ += up_ * delta * moveSpeed_;
        target_ = position_ + getForward();
        updateView();
    }
}

void Camera::updateView() {
    viewMatrix_ = glm::lookAt(position_, target_, up_);
}

void Camera::updateProjection() {
    projectionMatrix_ = glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_);
    // Vulkan clip space has inverted Y
    projectionMatrix_[1][1] *= -1;
}

void Camera::updateOrbitPosition() {
    float yawRad = glm::radians(yaw_);
    float pitchRad = glm::radians(pitch_);
    
    position_.x = target_.x + orbitDistance_ * cos(pitchRad) * cos(yawRad);
    position_.y = target_.y + orbitDistance_ * sin(pitchRad);
    position_.z = target_.z + orbitDistance_ * cos(pitchRad) * sin(yawRad);
    
    updateView();
}

void Camera::updateFPSVectors() {
    float yawRad = glm::radians(yaw_);
    float pitchRad = glm::radians(pitch_);
    
    glm::vec3 front;
    front.x = cos(yawRad) * cos(pitchRad);
    front.y = sin(pitchRad);
    front.z = sin(yawRad) * cos(pitchRad);
    front = glm::normalize(front);
    
    target_ = position_ + front;
    updateView();
}

} // namespace rs
