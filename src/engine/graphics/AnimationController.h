#pragma once
#include "Animation.h"
#include "Sprite.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace Engine {

class AnimationController {
public:
    explicit AnimationController(Sprite* sprite);
    ~AnimationController() = default;

    void AddAnimation(const std::string& name, std::unique_ptr<Animation> animation);
    void Play(const std::string& name);
    void Stop();
    void Update(float dt);

    Animation* GetCurrentAnimation() { return currentAnimation; }
    bool IsPlaying() const { return currentAnimation && currentAnimation->IsPlaying(); }

private:
    Sprite* sprite;
    std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
    Animation* currentAnimation = nullptr;
};

}
