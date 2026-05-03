#pragma once
#include "Animation.h"
#include "Sprite.h"
#include <unordered_map>
#include <string>

namespace Engine {

class AnimationController {
public:
    explicit AnimationController(Sprite* sprite);
    ~AnimationController();

    void AddAnimation(const std::string& name, Animation* animation);
    void Play(const std::string& name);
    void Stop();
    void Update(float dt);

    Animation* GetCurrentAnimation() { return currentAnimation; }
    bool IsPlaying() const { return currentAnimation && currentAnimation->IsPlaying(); }

private:
    Sprite* sprite;
    std::unordered_map<std::string, Animation*> animations;
    Animation* currentAnimation = nullptr;
};

}
