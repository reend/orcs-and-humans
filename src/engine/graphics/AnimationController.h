#pragma once
#include "Animation.h"
#include "Sprite.h"
#include <map>
#include <string>

namespace Engine {

class AnimationController {
public:
    AnimationController(Sprite* sprite);
    ~AnimationController() = default;
    
    void AddAnimation(const std::string& name, Animation* animation);
    void Play(const std::string& name);
    void Stop();
    void Update(float deltaTime);
    
    Animation* GetAnimation(const std::string& name);
    Animation* GetCurrentAnimation() { return currentAnimation; }
    const std::string& GetCurrentAnimationName() const { return currentAnimationName; }
    
    bool IsPlaying() const { return currentAnimation && currentAnimation->IsPlaying(); }
    
private:
    Sprite* sprite;
    std::map<std::string, Animation*> animations;
    Animation* currentAnimation;
    std::string currentAnimationName;
};

}
