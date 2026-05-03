#include "AnimationController.h"
#include "engine/logging/Logger.h"

namespace Engine {

AnimationController::AnimationController(Sprite* sprite)
    : sprite(sprite) {}

AnimationController::~AnimationController() {
    for (auto& [name, anim] : animations)
        delete anim;
}

void AnimationController::AddAnimation(const std::string& name, Animation* animation) {
    animations[name] = animation;
}

void AnimationController::Play(const std::string& name) {
    auto it = animations.find(name);
    if (it == animations.end()) {
        LOG_WARN("Animation not found: %s", name.c_str());
        return;
    }
    if (currentAnimation == it->second) return;

    if (currentAnimation) currentAnimation->Stop();
    currentAnimation = it->second;
    currentAnimation->Reset();
    currentAnimation->Play();
}

void AnimationController::Stop() {
    if (currentAnimation) currentAnimation->Stop();
}

void AnimationController::Update(float dt) {
    if (!currentAnimation) return;
    currentAnimation->Update(dt);
    if (sprite) sprite->SetSourceRect(currentAnimation->GetCurrentFrame());
}

}
