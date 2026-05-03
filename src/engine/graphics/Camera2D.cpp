#include "Camera2D.h"
#include <algorithm>

namespace Engine {

Camera2D::Camera2D(float x, float y) {
    camera.target   = {x, y};
    camera.offset   = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom     = 1.0f;
}

void Camera2D::SetPosition(raylib::Vector2 position) {
    camera.target = position;
    ClampToBounds();
}

void Camera2D::Move(raylib::Vector2 offset) {
    camera.target.x += offset.x;
    camera.target.y += offset.y;
    ClampToBounds();
}

void Camera2D::SetZoom(float zoom) {
    camera.zoom = std::clamp(zoom, 0.1f, 5.0f);
}

void Camera2D::SetBounds(float minX, float minY, float maxX, float maxY) {
    this->minX = minX; this->minY = minY;
    this->maxX = maxX; this->maxY = maxY;
}

void Camera2D::EnableBounds(bool enable) {
    boundsEnabled = enable;
}

void Camera2D::ClampToBounds() {
    if (!boundsEnabled) return;
    camera.target.x = std::clamp(camera.target.x, minX, maxX);
    camera.target.y = std::clamp(camera.target.y, minY, maxY);
}

raylib::Vector2 Camera2D::ScreenToWorld(raylib::Vector2 screenPos) const {
    return ::GetScreenToWorld2D(screenPos, camera);
}

raylib::Vector2 Camera2D::WorldToScreen(raylib::Vector2 worldPos) const {
    return ::GetWorldToScreen2D(worldPos, camera);
}

void Camera2D::Begin() { ::BeginMode2D(camera); }
void Camera2D::End()   { ::EndMode2D(); }

}
