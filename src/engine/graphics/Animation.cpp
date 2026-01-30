#include "Animation.h"

namespace Engine {

Animation::Animation() 
    : currentFrame(0)
    , frameTime(0.1f)
    , timeSinceLastFrame(0.0f)
    , playing(false)
    , loop(true)
    , finished(false) {
}

Animation::Animation(const std::vector<raylib::Rectangle>& frames, float fps, bool loop)
    : frames(frames)
    , currentFrame(0)
    , frameTime(1.0f / fps)
    , timeSinceLastFrame(0.0f)
    , playing(false)
    , loop(loop)
    , finished(false) {
}

void Animation::AddFrame(raylib::Rectangle frame) {
    frames.push_back(frame);
}

void Animation::SetFrames(const std::vector<raylib::Rectangle>& newFrames) {
    frames = newFrames;
    Reset();
}

void Animation::SetFPS(float fps) {
    frameTime = 1.0f / fps;
}

void Animation::SetLoop(bool shouldLoop) {
    loop = shouldLoop;
}

void Animation::Play() {
    playing = true;
    finished = false;
}

void Animation::Stop() {
    playing = false;
}

void Animation::Reset() {
    currentFrame = 0;
    timeSinceLastFrame = 0.0f;
    finished = false;
}

void Animation::Update(float deltaTime) {
    if (!playing || frames.empty() || finished) return;
    
    timeSinceLastFrame += deltaTime;
    
    if (timeSinceLastFrame >= frameTime) {
        timeSinceLastFrame = 0.0f;
        currentFrame++;
        
        if (currentFrame >= static_cast<int>(frames.size())) {
            if (loop) {
                currentFrame = 0;
            } else {
                currentFrame = static_cast<int>(frames.size()) - 1;
                finished = true;
                playing = false;
            }
        }
    }
}

raylib::Rectangle Animation::GetCurrentFrame() const {
    if (frames.empty()) {
        return raylib::Rectangle{0, 0, 0, 0};
    }
    return frames[currentFrame];
}

}
