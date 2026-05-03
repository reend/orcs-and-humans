#pragma once
#include "raylib-cpp.hpp"
#include <string>

namespace Engine {

class Sprite {
public:
    Sprite() = default;
    explicit Sprite(const std::string& texturePath);
    ~Sprite();

    bool Load(const std::string& texturePath);
    void Unload();

    void             SetSourceRect(raylib::Rectangle rect);
    raylib::Rectangle GetSourceRect() const { return sourceRect; }
    raylib::Vector2  GetSize()       const { return {sourceRect.width, sourceRect.height}; }
    bool             IsLoaded()      const { return texture.id > 0; }
    raylib::Texture2D* GetTexture()        { return &texture; }

private:
    raylib::Texture2D  texture    = {};
    raylib::Rectangle  sourceRect = {0, 0, 0, 0};
};

}
