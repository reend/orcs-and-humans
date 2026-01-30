#include "AnimationController.h"
#include "engine/logging/Logger.h"

namespace Engine {

AnimationController::AnimationController(Sprite* sprite)
    : sprite(sprite)
    , currentAnimation(nullptr) {
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
    
    if (currentAnimation != it->second) {
        if (currentAnimation) {
            currentAnimation->Stop();
        }
        
        currentAnimation = it->second;
        currentAnimationName = name;
        currentAnimation->Reset();
        currentAnimation->Play();
    }
}

void AnimationController::Stop() {
    if (currentAnimation) {
        currentAnimation->Stop();
    }
}

void AnimationController::Update(float deltaTime) {
    if (currentAnimation) {
        currentAnimation->Update(deltaTime);
        
        if (sprite) {
            sprite->SetSourceRect(currentAnimation->GetCurrentFrame());
        }
    }
}

Animation* AnimationController::GetAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        return it->second;
    }
    return nullptr;
}

}
