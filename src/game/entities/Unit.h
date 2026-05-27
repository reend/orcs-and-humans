#pragma once
#include "engine/graphics/Sprite.h"
#include "engine/graphics/AnimationController.h"
#include "raylib-cpp.hpp"
#include <vector>
#include <memory>
#include "game/GameTypes.h"

struct UnitStats {
    const char* name   = "Unit";
    int   level        = 1;
    int   hp           = 0;
    int   maxHp        = 0;
    int   armor        = 0;
    int   dmgMin       = 1;
    int   dmgMax       = 5;
    int   range        = 1;
    int   sight        = 4;
    int   speed        = 10;
    Rectangle   portraitFrame = {};
};

enum class Direction {
    Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft
};

class Unit {
public:
    explicit Unit(raylib::Vector2 position);

    void SetPath(const std::vector<raylib::Vector2>& path);
    void Update(float dt);
    void Draw();

    raylib::Vector2 GetPosition() const { return position; }
    bool IsMoving()   const { return !path.empty(); }
    bool IsSelected() const { return selected; }
    void SetSelected(bool value) { selected = value; }

    void PushBy(raylib::Vector2 offset);
    void DrawShadow();

    UnitStats GetStats() const;
    std::vector<ActionButton> GetActions() const;

private:
    int hp    = 30;
    int maxHp = 30;
    void LoadAnimations();
    void UpdateMovement(float dt);
    Direction GetDirectionToTarget(raylib::Vector2 target) const;
    void SetDirection(Direction dir);

    raylib::Vector2 position;
    std::vector<raylib::Vector2> path;
    int   currentPathIndex = 0;
    float speed            = 90.0f;
    Direction currentDirection = Direction::Down;
    bool  selected         = false;

    std::unique_ptr<Engine::Sprite>             sprite;
    std::unique_ptr<Engine::AnimationController> animController;
};
