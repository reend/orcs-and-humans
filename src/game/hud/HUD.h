#pragma once
#include "game/entities/Unit.h"
#include "game/entities/Building.h"
#include "game/GameConstants.h"
#include "raylib.h"

class HUD {
public:
    void Init();
    void Shutdown();
    void Update(Unit* unit, Building* building);
    void Render(Unit* unit, Building* building);

    ActionType PopPendingAction();

private:
    void DrawPanel();
    void DrawUnitInfo(Unit* unit);
    void DrawBuildingInfo(Building* building);
    void DrawActionButtons(const std::vector<ActionButton>& actions);
    Rectangle GetButtonRect(int col, int row) const;

    Texture2D  panel  = {};
    Texture2D  icons  = {};
    ActionType pendingAction = ActionType::None;
};