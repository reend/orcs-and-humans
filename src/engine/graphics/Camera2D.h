#pragma once
#include "raylib-cpp.hpp"

namespace Engine {

class Camera2D {
public:
    Camera2D(float x = 0.0f, float y = 0.0f);
    ~Camera2D() = default;
    
    void SetPosition(raylib::Vector2 position);
    void Move(raylib::Vector2 offset);
    void SetZoom(float zoom);
    
    void SetBounds(float minX, float minY, float maxX, float maxY);
    void EnableBounds(bool enable);
    
    raylib::Vector2 GetPosition() const { return position; }
    float GetZoom() const { return zoom; }
    
    raylib::Vector2 ScreenToWorld(raylib::Vector2 screenPos) const;
    raylib::Vector2 WorldToScreen(raylib::Vector2 worldPos) const;
    
    void Begin();
    void End();
    
private:
    void ClampToBounds();
    
    raylib::Vector2 position;
    raylib::Vector2 offset;
    float zoom;
    float rotation;
    
    bool boundsEnabled;
    float minX, minY, maxX, maxY;
    
    raylib::Camera2D camera;
};

}
