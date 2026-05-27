# Orcs & Humans — Warcraft II Engine Clone

A from-scratch Warcraft II-style game engine built in C++ using [raylib](https://www.raylib.com/) and [raylib-cpp](https://github.com/RobLoach/raylib-cpp).

---

## Build Instructions

### Requirements

- [CMake](https://cmake.org/) 3.14+
- [MSYS2](https://www.msys2.org/) with MinGW-w64 (ucrt64) **or** any GCC/Clang toolchain
- Git (CMake fetches raylib and raylib-cpp automatically)

### Steps

```bash
git clone https://github.com/your-username/orcs-and-humans.git
cd orcs-and-humans
./build.bat
```

The script configures CMake, builds the project, and places the executable in `build/warcraft.exe`.

> **Note:** On first build CMake will download raylib and raylib-cpp via `FetchContent`. This requires an internet connection.

---

## Assets

The project uses HD sprite rips from the MS-DOS version of **Warcraft II: Tides of Darkness**, uploaded by *Maxim* to [The Spriters Resource](https://www.spriters-resource.com/ms_dos/warcraftii/).

All assets must be placed in the `assets/` directory with the structure shown below.

### Required Assets

| Asset | Source | Path in project |
|---|---|---|
| Peon sprite sheet | [Peon — Spriters Resource](https://www.spriters-resource.com/ms_dos/warcraftii/asset/29480/) | `assets/units/orcs/peon/peon.png` |
| Winter tileset | [Winter Tiles — Spriters Resource](https://www.spriters-resource.com/ms_dos/warcraftii/asset/29503/) | `assets/winter/tileset.png` |
| Orc buildings | [Orc Buildings (Winter) — Spriters Resource](https://www.spriters-resource.com/ms_dos/warcraftii/asset/29543/) | `assets/buildings/orcs/buildings.png` |
| UI icons | [Icons — Spriters Resource](https://www.spriters-resource.com/ms_dos/warcraftii/asset/29500/) | `assets/ui/orcs/icons/icons.png` |
| Orc cursor | [WARCRAFT 2 ORC cursor set — RealWorld Designer](https://www.rw-designer.com/cursor-detail/207081) | `assets/ui/cursors/orc.png` |
| HUD background panel | Search for "Warcraft 2 Orc HUD" or "BG_Orc" HD rips | `assets/ui/orcs/bg/BG_Orc.png` |

### Map CSV Files

The tilemaps are loaded from CSV files exported from [Tiled Map Editor](https://www.mapeditor.org/):

| File | Description |
|---|---|
| `assets/winter/ground.csv` | Ground layer tile IDs |
| `assets/winter/forest.csv` | Forest/tree layer tile IDs |

Tileset settings in Tiled: **tile size 32×32**, **19 tiles per row**, **spacing 1**.

### Icons Tileset Settings (for reference when measuring coordinates)

When opening `icons.png` in Tiled to identify icon positions:
- Tile width: **46**, Tile height: **38**
- Margin: **6**, Spacing: **3**

---

## Project Structure

```
src/
├── engine/
│   ├── core/           Game loop, timing
│   ├── graphics/       Sprite, Animation, Tilemap, Camera, SpriteRenderer
│   ├── input/          Keyboard and mouse input
│   ├── pathfinding/    A* pathfinder with octile heuristic
│   └── physics/        Unit-to-unit collision resolution
└── game/
    ├── entities/       Unit, Building (with stats and action definitions)
    ├── hud/            HUD class — portrait, stats panel, action buttons
    ├── world/          World, Map — game state and tile management
    ├── GameConstants.h All named constants (screen, map, camera, input)
    └── GameTypes.h     ActionType and ActionButton shared types
```

---

## What Was Implemented

### Engine

- **Game loop** — fixed FPS, delta time via `Time` class
- **Sprite system** — texture loading, source rect animation frames
- **Animation controller** — named animations, FPS, looping, directional state machine
- **Tilemap** — CSV-driven ground and forest layers, frustum culling, passability grid
- **Camera** — `Camera2D` with zoom, pan (WASD), bounds clamping, screen↔world coordinate transforms
- **Sprite renderer** — batched draw calls sorted by texture to minimize GPU state changes
- **A\* pathfinder** — octile heuristic, 8-directional movement, corner-cutting prevention
- **Collision system** — unit-to-unit overlap separation with push vectors
- **Logger** — tagged log levels (DEBUG, INFO, WARN, ERROR)
- **Input abstraction** — `IsPressed` / `IsDown` / `IsReleased` wrappers

### Game

- **Units (Peon)** — 8-directional sprite animation, pathfinding movement, selection
- **Buildings (Great Hall)** — tile-placed, sprite-rendered, map passability blocking
- **Selection system** — click-to-select, drag-rectangle multi-select for units and buildings
- **Right-click move orders** — pathfinding to clicked tile with formation offsets for multiple units
- **HUD panel** — scaled `BG_Orc.png` side panel with:
  - Unit/building portrait from `icons.png`
  - HP bar and stat text (Armor, Damage, Range, Sight, Speed)
  - 3×3 action button grid with icons and hotkey labels
- **Contrast shader** — GLSL fragment shader applied to the map for visual depth
- **Named constants** — all magic numbers extracted into `GameConstants.h`
- **Y-sorting** — units and buildings sorted by world Y position for correct depth rendering

---

## Controls

| Input | Action |
|---|---|
| Left click | Select unit / building |
| Left drag | Rectangle multi-select |
| Right click | Move selected units to tile |
| W / A / S / D | Pan camera |

---

## Future Work

This project may be continued in the future. Potential areas of development:

- Minimap rendering in the HUD
- Resource system (Gold, Lumber, Oil)
- Unit training from buildings (Great Hall → Peon)
- Build menu with nested action submenus (Peon → Build → building list)
- Combat system (attack, damage, death animations)
- Fog of war
- Multiple unit types (Grunt, Axethrower, etc.)
- Sound effects and music
- Save / load game state
- Human faction with mirrored HUD
