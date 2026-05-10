#include "CollisionSystem.h"

namespace Engine {

void CollisionSystem::Resolve(std::vector<std::unique_ptr<Unit>>& units) {
    for (int i = 0; i < (int)units.size(); i++) {
        for (int j = i + 1; j < (int)units.size(); j++) {
            raylib::Vector2 a = units[i]->GetPosition();
            raylib::Vector2 b = units[j]->GetPosition();
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < MIN_DIST && dist > 0.0f) {
                bool aMoving = units[i]->IsMoving();
                bool bMoving = units[j]->IsMoving();
                float nx = dx / dist, ny = dy / dist; 
                float overlap = (MIN_DIST - dist) * 0.5f;
                if (aMoving) units[i]->PushBy({nx * overlap, ny * overlap});
                if (bMoving) units[j]->PushBy({-nx * overlap, -ny * overlap});
            }
        }
    }
}

}