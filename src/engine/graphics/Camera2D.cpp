#include "Camera2D.h"

namespace Engine {

Camera2D::Camera2D(float x, float y)
    : position(raylib::Vector2{x, y})
    , offset(raylib::Vector2{0, 0})
    , zoom(1.0f)
    , rotation(0.0f)
    , boundsEnabled(false)
    , minX(0), minY(0), maxX(0), maxY(0) {
    
    camera.target = position;
    camera.offset = offset;
    camera.rotation = rotation;
    camera.zoom = zoom;
}

void Camera2D::SetPosition(raylib::Vector2 newPosition) {
    position = newPosition;
    ClampToBounds();
    camera.target = position;
}

void Camera2D::Move(raylib::Vector2 offset) {
    position.x += offset.x;
    position.y += offset.y;
    ClampToBounds();
    camera.target = position;
}

void Camera2D::SetZoom(float newZoom) {
    if (newZoom < 0.1f) newZoom = 0.1f;
    if (newZoom > 5.0f) newZoom = 5.0f;
    
    zoom = newZoom;
    camera.zoom = zoom;
}

void Camera2D::SetBounds(float minX, float minY, float maxX, float maxY) {
    this->minX = minX;
    this->minY = minY;
    this->maxX = maxX;
    this->maxY = maxY;
}

void Camera2D::EnableBounds(bool enable) {
    boundsEnabled = enable;
}

void Camera2D::ClampToBounds() {
    if (!boundsEnabled) return;
    
    if (position.x < minX) position.x = minX;
    if (position.y < minY) position.y = minY;
    if (position.x > maxX) position.x = maxX;
    if (position.y > maxY) position.y = maxY;
}

raylib::Vector2 Camera2D::ScreenToWorld(raylib::Vector2 screenPos) const {
    return ::GetScreenToWorld2D(screenPos, camera);
}

raylib::Vector2 Camera2D::WorldToScreen(raylib::Vector2 worldPos) const {
    return ::GetWorldToScreen2D(worldPos, camera);
}

void Camera2D::Begin() {
    ::BeginMode2D(camera);
}

void Camera2D::End() {
    ::EndMode2D();
}

}
