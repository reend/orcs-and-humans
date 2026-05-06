#include "SpriteRenderer.h"
#include "engine/logging/Logger.h"
#include <algorithm>

namespace Engine {

SpriteRenderer::SpriteRenderer() {
    batches.reserve(1000);
}

void SpriteRenderer::Begin() {
    if (isDrawing) {
        LOG_WARN("SpriteRenderer::Begin() called while already drawing!");
        return;
    }
    
    batches.clear();
    batchCount = 0;
    drawCallCount = 0;
    isDrawing = true;
}

void SpriteRenderer::DrawSprite(
    raylib::Texture2D* texture,
    raylib::Rectangle sourceRect,
    raylib::Vector2 position,
    float rotation,
    float scale,
    raylib::Vector2 origin,
    raylib::Color tint
) {
    if (!isDrawing) {
        LOG_WARN("SpriteRenderer::DrawSprite() called outside Begin/End!");
        return;
    }
    
    if (!texture || texture->id == 0) {
        return;
    }
    
    raylib::Rectangle destRect = {
        position.x,
        position.y,
        sourceRect.width * scale,
        sourceRect.height * scale
    };
    
    SpriteBatch batch;
    batch.texture = texture;
    batch.sourceRect = sourceRect;
    batch.destRect = destRect;
    batch.origin = origin;
    batch.rotation = rotation;
    batch.tint = tint;
    
    batches.push_back(batch);
    batchCount++;
}

void SpriteRenderer::End() {
    if (!isDrawing) {
        LOG_WARN("SpriteRenderer::End() called without Begin!");
        return;
    }
    
    Flush();
    isDrawing = false;
}

void SpriteRenderer::Flush() {
    if (batches.empty()) return;
    
    std::sort(batches.begin(), batches.end(), 
        [](const SpriteBatch& a, const SpriteBatch& b) {
            return a.texture->id < b.texture->id;
        }
    );
    
    raylib::Texture2D* currentTexture = nullptr;
    
    for (const auto& batch : batches) {
        if (currentTexture != batch.texture) {
            currentTexture = batch.texture;
            drawCallCount++;
        }
        
        ::DrawTexturePro(
            *batch.texture,
            batch.sourceRect,
            batch.destRect,
            batch.origin,
            batch.rotation,
            batch.tint
        );
    }
}

}
