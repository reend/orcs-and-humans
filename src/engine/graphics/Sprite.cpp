#include "Sprite.h"
#include "engine/logging/Logger.h"

namespace Engine {

Sprite::Sprite(const std::string& texturePath) {
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
    sourceRect = raylib::Rectangle{0, 0, (float)texture.width, (float)texture.height};
    LOG_DEBUG("Loaded texture: %s (%dx%d)", texturePath.c_str(), texture.width, texture.height);
    return true;
}

void Sprite::Unload() {
    if (IsLoaded()) {
        ::UnloadTexture(texture);
        texture.id = 0;
    }
}

void Sprite::SetSourceRect(raylib::Rectangle rect) {
    sourceRect = rect;
}

}
