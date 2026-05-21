#include "Animation.h"

namespace Engine {

void Animation::AddFrame(raylib::Rectangle frame) {
    frames.push_back(frame);
}

void Animation::SetFPS(float fps) {
    frameTime = 1.0f / fps;
}

void Animation::SetLoop(bool shouldLoop) {
    loop = shouldLoop;
}

void Animation::Play() {
    playing = true;
}

void Animation::Stop() {
    playing = false;
}

void Animation::Reset() {
    currentFrame         = 0;
    timeSinceLastFrame   = 0.0f;
}

void Animation::Update(float dt) {
    if (!playing || frames.empty()) return;

    timeSinceLastFrame += dt;
    if (timeSinceLastFrame < frameTime) return;

    timeSinceLastFrame -= frameTime;
    currentFrame++;

    if (currentFrame >= (int)frames.size()) {
        if (loop) currentFrame = 0;
        else { currentFrame = (int)frames.size() - 1; playing = false; }
    }
}

raylib::Rectangle Animation::GetCurrentFrame() const {
    if (frames.empty()) return {0, 0, 0, 0};
    return frames[currentFrame];
}

}
