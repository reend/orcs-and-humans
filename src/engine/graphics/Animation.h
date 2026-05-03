#pragma once
#include "raylib-cpp.hpp"
#include <vector>

namespace Engine {

class Animation {
public:
    Animation() = default;

    void AddFrame(raylib::Rectangle frame);
    void SetFPS(float fps);
    void SetLoop(bool loop);

    void Play();
    void Stop();
    void Reset();
    void Update(float dt);

    raylib::Rectangle GetCurrentFrame() const;
    int  GetFrameCount() const { return (int)frames.size(); }
    bool IsPlaying()    const { return playing; }

private:
    std::vector<raylib::Rectangle> frames;
    int   currentFrame        = 0;
    float frameTime           = 0.1f;
    float timeSinceLastFrame  = 0.0f;
    bool  playing             = false;
    bool  loop                = true;
};

}
