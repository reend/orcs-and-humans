#pragma once
#include "raylib-cpp.hpp"

namespace Engine {

class Camera2D {
public:
    Camera2D(float x = 0.0f, float y = 0.0f);

    void SetPosition(raylib::Vector2 position);
    void Move(raylib::Vector2 offset);
    void SetZoom(float zoom);

    void SetBounds(float minX, float minY, float maxX, float maxY);
    void EnableBounds(bool enable);

    raylib::Vector2 GetPosition() const { return camera.target; }
    float           GetZoom()     const { return camera.zoom;   }

    raylib::Vector2 ScreenToWorld(raylib::Vector2 screenPos) const;
    raylib::Vector2 WorldToScreen(raylib::Vector2 worldPos)  const;

    void SetOffset(raylib::Vector2 offset);

    void Begin();
    void End();

private:
    void ClampToBounds();

    raylib::Camera2D camera;
    bool  boundsEnabled = false;
    float minX = 0;
    float minY = 0;
    float maxX = 0;
    float maxY = 0;
};

}
