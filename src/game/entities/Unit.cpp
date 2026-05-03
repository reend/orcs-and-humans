#include "Unit.h"
#include "engine/logging/Logger.h"
#include <cmath>

Unit::Unit(raylib::Vector2 position)
    : position(position)
    , currentDirection(Direction::Down) {
    sprite         = std::make_unique<Engine::Sprite>("assets/units/orcs/peon/peon.png");
    animController = std::make_unique<Engine::AnimationController>(sprite.get());
    LoadAnimations();
    animController->Play("idle_down");
    LOG_INFO("Unit created at (%.1f, %.1f)", position.x, position.y);
}

void Unit::LoadAnimations() {
    struct AnimDef { const char* name; float x; bool walk; };

    constexpr float W = 51, H = 40;
    constexpr AnimDef cols[] = {
        {"up",        0,   false},
        {"upright",   51,  false},
        {"right",     102, false},
        {"downright", 153, false},
        {"down",      204, false},
    };

    for (const auto& col : cols) {
        auto* idle = new Engine::Animation();
        idle->AddFrame({col.x, 0, W, H});
        idle->SetLoop(true);
        animController->AddAnimation(std::string("idle_") + col.name, idle);

        auto* walk = new Engine::Animation();
        for (int row = 1; row <= 4; row++)
            walk->AddFrame({col.x, static_cast<float>(row) * H, W, H});
        walk->SetFPS(8);
        walk->SetLoop(true);
        animController->AddAnimation(std::string("walk_") + col.name, walk);
    }

    LOG_DEBUG("Unit animations loaded");
}

void Unit::SetPath(const std::vector<raylib::Vector2>& newPath) {
    path              = newPath;
    currentPathIndex  = 0;
    if (!path.empty())
        SetDirection(GetDirectionToTarget(path[0]));
}

void Unit::Update(float dt) {
    UpdateMovement(dt);
    animController->Update(dt);
}

void Unit::UpdateMovement(float dt) {
    if (path.empty()) return;

    raylib::Vector2 target = path[currentPathIndex];
    float dx = target.x - position.x;
    float dy = target.y - position.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 2.0f) {
        if (++currentPathIndex >= (int)path.size()) {
            path.clear();
            currentPathIndex = 0;
            const char* suffix[] = {"up","upright","right","downright","down","downright","right","upright"};
            animController->Play(std::string("idle_") + suffix[(int)currentDirection]);
            return;
        }
        SetDirection(GetDirectionToTarget(path[currentPathIndex]));
        return;
    }

    position.x += (dx / dist) * speed * dt;
    position.y += (dy / dist) * speed * dt;
}

Direction Unit::GetDirectionToTarget(raylib::Vector2 target) const {
    float angle = std::atan2(target.y - position.y, target.x - position.x) * 180.0f / 3.14159f;
    if (angle < 0) angle += 360.0f;

    if (angle >= 337.5f || angle < 22.5f)  return Direction::Right;
    if (angle < 67.5f)                     return Direction::DownRight;
    if (angle < 112.5f)                    return Direction::Down;
    if (angle < 157.5f)                    return Direction::DownLeft;
    if (angle < 202.5f)                    return Direction::Left;
    if (angle < 247.5f)                    return Direction::UpLeft;
    if (angle < 292.5f)                    return Direction::Up;
    return Direction::UpRight;
}

void Unit::SetDirection(Direction dir) {
    if (dir == currentDirection) return;
    currentDirection = dir;

    const char* suffix[] = {"up","upright","right","downright","down","downright","right","upright"};
    animController->Play(std::string("walk_") + suffix[(int)dir]);
}

void Unit::Draw() {
    bool flip = (currentDirection == Direction::Left   ||
                 currentDirection == Direction::DownLeft ||
                 currentDirection == Direction::UpLeft);

    raylib::Rectangle src = animController->GetCurrentAnimation()->GetCurrentFrame();
    if (flip) src.width = -src.width;

    DrawTexturePro(
        *sprite->GetTexture(),
        src,
        {position.x, position.y, 51, 40},
        {0, 0},
        0.0f,
        WHITE
    );

    if (selected)
        DrawCircleLines((int)(position.x + 25), (int)(position.y + 35), 14.0f, GREEN);
}
