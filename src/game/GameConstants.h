#pragma once

static constexpr int   SCREEN_W    = 1920;
static constexpr int   SCREEN_H    = 1080;

static constexpr float HUD_SRC_W   = 855.0f;
static constexpr float HUD_SRC_H   = 2160.0f;
static constexpr float HUD_W       = HUD_SRC_W / HUD_SRC_H * SCREEN_H; // ~427px

static constexpr float CAM_SPEED = 400.0f;