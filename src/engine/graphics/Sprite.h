#pragma once
#include "raylib-cpp.hpp"
#include <string>

namespace Engine {

class Sprite {
public:
    Sprite();
    Sprite(const std::string& texturePath);
    Sprite(const std::string& texturePath, raylib::Rectangle sourceRect);
    ~Sprite();

    bool Load(const std::string& texturePath);
    void Unload();
    
    void Draw(raylib::Vector2 position, float rotation = 0.0f, float scale = 1.0f);
    void Draw(raylib::Vector2 position, raylib::Vector2 origin, float rotation = 0.0f, float scale = 1.0f);
    
    void SetSourceRect(raylib::Rectangle rect);
    raylib::Rectangle GetSourceRect() const { return sourceRect; }
    
    void SetTint(raylib::Color color) { tint = color; }
    raylib::Color GetTint() const { return tint; }
    
    raylib::Vector2 GetSize() const;
    bool IsLoaded() const { return texture.id > 0; }
    
private:
    raylib::Texture2D texture;
    raylib::Rectangle sourceRect;
    raylib::Color tint;
};

}
