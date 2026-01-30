#pragma once
#include "raylib-cpp.hpp"
#include <vector>
#include <string>

namespace Engine {

class Animation {
public:
    Animation();
    Animation(const std::vector<raylib::Rectangle>& frames, float fps, bool loop = true);
    
    void AddFrame(raylib::Rectangle frame);
    void SetFrames(const std::vector<raylib::Rectangle>& frames);
    void SetFPS(float fps);
    void SetLoop(bool loop);
    
    void Play();
    void Stop();
    void Reset();
    void Update(float deltaTime);
    
    raylib::Rectangle GetCurrentFrame() const;
    int GetCurrentFrameIndex() const { return currentFrame; }
    int GetFrameCount() const { return static_cast<int>(frames.size()); }
    
    bool IsPlaying() const { return playing; }
    bool IsFinished() const { return finished; }
    
private:
    std::vector<raylib::Rectangle> frames;
    int currentFrame;
    float frameTime;
    float timeSinceLastFrame;
    bool playing;
    bool loop;
    bool finished;
};

}
