#include "Unit.h"
#include "engine/logging/Logger.h"
#include <cmath>

static constexpr float SPRITE_W    = 51.0f;
static constexpr float SPRITE_H    = 40.0f;
static constexpr float SEL_OFFS_X  = 25.0f;
static constexpr float SEL_OFFS_Y  = 35.0f;
static constexpr float SEL_RADIUS  = 14.0f;

static constexpr const char* DIR_SUFFIX[] = {
    "up", "upright", "right", "downright", "down", "downright", "right", "upright"
};

Unit::Unit(raylib::Vector2 position)
    : position(position)
    , currentDirection(Direction::Down) {
    sprite         = std::make_unique<Engine::Sprite>("assets/units/orcs/peon/peon.png");
    animController = std::make_unique<Engine::AnimationController>(sprite.get());
    LoadAnimations();
    animController->Play("idle_down");
    LOG_DEBUG("Unit created at (%.1f, %.1f)", position.x, position.y);
}

void Unit::LoadAnimations() {
    struct AnimDef { const char* name; float x; };

    constexpr AnimDef cols[] = {
        {"up",        0},
        {"upright",   SPRITE_W},
        {"right",     SPRITE_W * 2},
        {"downright", SPRITE_W * 3},
        {"down",      SPRITE_W * 4},
    };

    for (const auto& col : cols) {
        auto* idle = new Engine::Animation();
        idle->AddFrame({col.x, 0, SPRITE_W, SPRITE_H});
        idle->SetLoop(true);
        animController->AddAnimation(std::string("idle_") + col.name, idle);

        auto* walk = new Engine::Animation();
        for (int row = 1; row <= 4; row++)
            walk->AddFrame({col.x, row * SPRITE_H, SPRITE_W, SPRITE_H});
        walk->SetFPS(8);
        walk->SetLoop(true);
        animController->AddAnimation(std::string("walk_") + col.name, walk);
    }
}

void Unit::SetPath(const std::vector<raylib::Vector2>& newPath) {
    path             = newPath;
    currentPathIndex = 0;
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
    float dx   = target.x - position.x;
    float dy   = target.y - position.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 2.0f) {
        if (++currentPathIndex >= (int)path.size()) {
            path.clear();
            currentPathIndex = 0;
            animController->Play(std::string("idle_") + DIR_SUFFIX[(int)currentDirection]);
            return;
        }
        SetDirection(GetDirectionToTarget(path[currentPathIndex]));
        return;
    }

    position.x += (dx / dist) * speed * dt;
    position.y += (dy / dist) * speed * dt;
}

Direction Unit::GetDirectionToTarget(raylib::Vector2 target) const {
    float angle = std::atan2(target.y - position.y, target.x - position.x) * RAD2DEG;
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
    animController->Play(std::string("walk_") + DIR_SUFFIX[(int)dir]);
}

void Unit::Draw() {
    bool flip = (currentDirection == Direction::Left    ||
                 currentDirection == Direction::DownLeft ||
                 currentDirection == Direction::UpLeft);

    raylib::Rectangle src = animController->GetCurrentAnimation()->GetCurrentFrame();
    if (flip) src.width = -src.width;

    DrawTexturePro(
        *sprite->GetTexture(),
        src,
        {position.x, position.y, SPRITE_W, SPRITE_H},
        {0, 0},
        0.0f,
        WHITE
    );

    if (selected)
        DrawCircleLines((int)(position.x + SEL_OFFS_X), (int)(position.y + SEL_OFFS_Y), SEL_RADIUS, GREEN);
}
