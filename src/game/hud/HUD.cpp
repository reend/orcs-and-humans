#include "HUD.h"
#include "engine/logging/Logger.h"
#include <cstdio>
#include <algorithm>

static constexpr int HUD_PORT_X      = 10;
static constexpr int HUD_PORT_Y      = 371;
static constexpr int HUD_PORT_W      = 116; 
static constexpr int HUD_PORT_H      = 98;

static constexpr int HUD_NAME_X      = 135;
static constexpr int HUD_NAME_Y      = 375;
static constexpr int HUD_NAME_FONT   = 22;

static constexpr int HUD_HP_X        = 10;
static constexpr int HUD_HP_Y        = 478;
static constexpr int HUD_HP_W        = 116;  
static constexpr int HUD_HP_H        = 14;

static constexpr int HUD_STAT_X      = 12;  
static constexpr int HUD_STAT_Y      = 505;
static constexpr int HUD_STAT_LINE   = 28;
static constexpr int HUD_STAT_FONT   = 20;

static constexpr int HUD_BTN_COLS    = 3;
static constexpr int HUD_BTN_X       = 23;  
static constexpr int HUD_BTN_Y       = 766;  
static constexpr int HUD_BTN_W       = 106;
static constexpr int HUD_BTN_H       = 88;
static constexpr int HUD_BTN_PAD     = 10;

void HUD::Init() {
    panel = LoadTexture("assets/ui/orcs/bg/BG_Orc.png");
    icons = LoadTexture("assets/ui/orcs/icons/icons.png");
}

void HUD::Shutdown() {
    UnloadTexture(panel);
    UnloadTexture(icons);
}

ActionType HUD::PopPendingAction() {
    ActionType a = pendingAction;
    pendingAction = ActionType::None;
    return a;
}

Rectangle HUD::GetButtonRect(int col, int row) const {
    return {
        (float)(HUD_BTN_X + col * (HUD_BTN_W + HUD_BTN_PAD)),
        (float)(HUD_BTN_Y + row * (HUD_BTN_H + HUD_BTN_PAD)),
        (float)HUD_BTN_W,
        (float)HUD_BTN_H
    };
}

void HUD::DrawPanel() {
    Rectangle src  = {HUD_SRC_X, HUD_SRC_Y, HUD_SRC_W, HUD_SRC_H};
    Rectangle dest = {0, 0, HUD_W, (float)SCREEN_H};
    DrawTexturePro(panel, src, dest, {0, 0}, 0.0f, WHITE);
}

void HUD::DrawActionButtons(const std::vector<ActionButton>& actions) {
    for (int i = 0; i < (int)actions.size(); i++) {
        int col = i % HUD_BTN_COLS;
        int row = i / HUD_BTN_COLS;
        Rectangle rect = GetButtonRect(col, row);

        DrawTexturePro(icons, actions[i].iconRect, rect, {0, 0}, 0.0f, WHITE);

        char key[2] = {actions[i].hotkey, '\0'};
        DrawText(key, (int)rect.x + 5, (int)rect.y + 5, 16, YELLOW);
    }
}

void HUD::DrawUnitInfo(Unit* unit) {
    UnitStats s = unit->GetStats();

    Rectangle destPort = {(float)HUD_PORT_X, (float)HUD_PORT_Y,
                          (float)HUD_PORT_W,  (float)HUD_PORT_H};
    DrawTexturePro(icons, s.portraitFrame, destPort, {0, 0}, 0.0f, WHITE);
    DrawRectangleLinesEx(destPort, 2.0f, DARKGRAY);

    DrawText(s.name, HUD_NAME_X, HUD_NAME_Y, HUD_NAME_FONT, WHITE);
    char lvl[32];
    std::snprintf(lvl, sizeof(lvl), "Level %d", s.level);
    DrawText(lvl, HUD_NAME_X, HUD_NAME_Y + HUD_NAME_FONT + 4, HUD_STAT_FONT, LIGHTGRAY);

    DrawRectangle(HUD_HP_X, HUD_HP_Y, HUD_HP_W, HUD_HP_H, DARKGRAY);
    float ratio = (float)s.hp / (float)s.maxHp;
    DrawRectangle(HUD_HP_X, HUD_HP_Y, (int)(HUD_HP_W * ratio), HUD_HP_H, GREEN);
    char hpText[32];
    std::snprintf(hpText, sizeof(hpText), "%d/%d", s.hp, s.maxHp);
    DrawText(hpText, HUD_HP_X + HUD_HP_W / 2 - 20, HUD_HP_Y - 16, 14, WHITE);

    char buf[64];
    int y = HUD_STAT_Y;
    std::snprintf(buf, sizeof(buf), "Armor:  %d",     s.armor);          DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, LIGHTGRAY); y += HUD_STAT_LINE;
    std::snprintf(buf, sizeof(buf), "Damage: %d-%d",  s.dmgMin, s.dmgMax); DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, LIGHTGRAY); y += HUD_STAT_LINE;
    std::snprintf(buf, sizeof(buf), "Range:  %d",     s.range);           DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, LIGHTGRAY); y += HUD_STAT_LINE;
    std::snprintf(buf, sizeof(buf), "Sight:  %d",     s.sight);           DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, LIGHTGRAY); y += HUD_STAT_LINE;
    std::snprintf(buf, sizeof(buf), "Speed:  %d",     s.speed);           DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, LIGHTGRAY);
}

void HUD::DrawBuildingInfo(Building* building) {
    BuildingStats s = building->GetStats();

    Rectangle destPort = {(float)HUD_PORT_X, (float)HUD_PORT_Y,
                          (float)HUD_PORT_W,  (float)HUD_PORT_H};
    DrawTexturePro(icons, s.portraitFrame, destPort, {0, 0}, 0.0f, WHITE);
    DrawRectangleLinesEx(destPort, 2.0f, DARKGRAY);

    DrawText(s.name, HUD_NAME_X, HUD_NAME_Y, HUD_NAME_FONT, WHITE);

    DrawRectangle(HUD_HP_X, HUD_HP_Y, HUD_HP_W, HUD_HP_H, DARKGRAY);
    float ratio = (float)s.hp / (float)s.maxHp;
    DrawRectangle(HUD_HP_X, HUD_HP_Y, (int)(HUD_HP_W * ratio), HUD_HP_H, GREEN);
    char hpText[32];
    std::snprintf(hpText, sizeof(hpText), "%d/%d", s.hp, s.maxHp);
    DrawText(hpText, HUD_HP_X + HUD_HP_W / 2 - 20, HUD_HP_Y - 16, 14, WHITE);

    char buf[64];
    int y = HUD_STAT_Y;
    std::snprintf(buf, sizeof(buf), "Gold:   %d", s.goldCost);   DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, YELLOW);     y += HUD_STAT_LINE;
    std::snprintf(buf, sizeof(buf), "Lumber: %d", s.lumberCost); DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, GREEN);      y += HUD_STAT_LINE;
    std::snprintf(buf, sizeof(buf), "Oil:    %d", s.oilCost);    DrawText(buf, HUD_STAT_X, y, HUD_STAT_FONT, LIGHTGRAY);
}

void HUD::Update(Unit* unit, Building* building) {
    if (!unit && !building) return;
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    Vector2 mouse = GetMousePosition();
    if (mouse.x >= HUD_W) return;

    auto actions = unit ? unit->GetActions() : building->GetActions();
    for (int i = 0; i < (int)actions.size(); i++) {
        if (CheckCollisionPointRec(mouse, GetButtonRect(i % HUD_BTN_COLS, i / HUD_BTN_COLS))) {
            pendingAction = actions[i].type;
            LOG_DEBUG("HUD action: %s", actions[i].label);
            return;
        }
    }
}

void HUD::Render(Unit* unit, Building* building) {
    DrawPanel();
    if (!unit && !building) return;

    auto actions = unit ? unit->GetActions() : building->GetActions();
    if (unit)     DrawUnitInfo(unit);
    else          DrawBuildingInfo(building);
    DrawActionButtons(actions);
}