#pragma once
#include "raylib-cpp.hpp"
#include <vector>

namespace Engine {

struct SpriteBatch {
    raylib::Texture2D* texture;
    raylib::Rectangle sourceRect;
    raylib::Rectangle destRect;
    raylib::Vector2 origin;
    float rotation;
    raylib::Color tint;
};

class SpriteRenderer {
public:
    SpriteRenderer();

    void Begin();
    void DrawSprite(
        raylib::Texture2D* texture,
        raylib::Rectangle sourceRect,
        raylib::Vector2 position,
        float rotation = 0.0f,
        float scale = 1.0f,
        raylib::Vector2 origin = {0, 0},
        raylib::Color tint = WHITE
    );
    void End();
    
    int GetBatchCount()    const { return batchCount;    }
    int GetDrawCallCount() const { return drawCallCount; }

private:
    void Flush();
    std::vector<SpriteBatch> batches;
    int  batchCount    = 0;
    int  drawCallCount = 0;
    bool isDrawing     = false;
};

}
