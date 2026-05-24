#pragma once

// Screen
static constexpr int   SCREEN_W              = 1920;
static constexpr int   SCREEN_H              = 1080;

// HUD panel (BG_Orc.png, frame TopLeft from BG_Orc.json)
static constexpr float HUD_SRC_X             = 1.0f;
static constexpr float HUD_SRC_Y             = 1.0f;
static constexpr float HUD_SRC_W             = 855.0f;
static constexpr float HUD_SRC_H             = 2160.0f;
static constexpr float HUD_W                 = HUD_SRC_W / HUD_SRC_H * SCREEN_H; // ~427px

// Map
static constexpr int   MAP_WIDTH             = 128;
static constexpr int   MAP_HEIGHT            = 128;
static constexpr int   TILE_SIZE             = 32;

// Tileset
static constexpr int   TILESET_TILES_PER_ROW = 19;
static constexpr int   TILESET_SPACING       = 1;

// Camera
static constexpr float CAM_SPEED             = 400.0f;
static constexpr float CAM_ZOOM              = 2.1f;
static constexpr float CAM_CONTRAST          = 1.1f;

// Input
static constexpr float CLICK_THRESHOLD       = 5.0f;
static constexpr float UNIT_CLICK_RADIUS     = 60.0f;
