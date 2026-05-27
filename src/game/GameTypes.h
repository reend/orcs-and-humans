#pragma once
#include "raylib.h"

enum class ActionType {
    None,
    Move, Stop, Attack,
    Repair, Harvest, Build,
    Train,
};

struct ActionButton {
    ActionType  type;
    char        hotkey;
    const char* label;
    Rectangle   iconRect;
};