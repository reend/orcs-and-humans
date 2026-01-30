#include "Sprite.h"
#include "engine/logging/Logger.h"

namespace Engine {

Sprite::Sprite() 
    : sourceRect(raylib::Rectangle{0, 0, 0, 0}), tint(WHITE) {
}

Sprite::Sprite(const std::string& texturePath) 
    : Sprite() {
    Load(texturePath);
}

Sprite::Sprite(const std::string& texturePath, raylib::Rectangle sourceRect) 
    : sourceRect(sourceRect), tint(WHITE) {
    Load(texturePath);
}

Sprite::~Sprite() {
    Unload();
}

bool Sprite::Load(const std::string& texturePath) {
    Unload();
    
    texture = ::LoadTexture(texturePath.c_str());
    
    if (texture.id == 0) {
        LOG_ERROR("Failed to load texture: %s", texturePath.c_str());
        return false;
    }
    
    if (sourceRect.width == 0 && sourceRect.height == 0) {
        sourceRect = raylib::Rectangle{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    }
    
    LOG_DEBUG("Loaded texture: %s (%dx%d)", texturePath.c_str(), texture.width, texture.height);
    return true;
}

void Sprite::Unload() {
    if (IsLoaded()) {
        ::UnloadTexture(texture);
        texture.id = 0;
    }
}

void Sprite::Draw(raylib::Vector2 position, float rotation, float scale) {
    if (!IsLoaded()) return;
    
    raylib::Rectangle destRect = {
        position.x,
        position.y,
        sourceRect.width * scale,
        sourceRect.height * scale
    };
    
    raylib::Vector2 origin = {0, 0};
    
    ::DrawTexturePro(texture, sourceRect, destRect, origin, rotation, tint);
}

void Sprite::Draw(raylib::Vector2 position, raylib::Vector2 origin, float rotation, float scale) {
    if (!IsLoaded()) return;
    
    raylib::Rectangle destRect = {
        position.x,
        position.y,
        sourceRect.width * scale,
        sourceRect.height * scale
    };
    
    ::DrawTexturePro(texture, sourceRect, destRect, origin, rotation, tint);
}

void Sprite::SetSourceRect(raylib::Rectangle rect) {
    sourceRect = rect;
}

raylib::Vector2 Sprite::GetSize() const {
    return {sourceRect.width, sourceRect.height};
}

}
