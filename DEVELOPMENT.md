# AI Development Context

This file contains project context for AI assistants working on this codebase.

## Project Overview

**Name:** Warcraft Engine  
**Goal:** RTS game engine clone of Warcraft 2  
**Language:** C++17  
**Platform:** Windows-only  
**Graphics:** raylib 5.0 + raylib-cpp v5.0.1  

## References

- **Primary:** [Hazel Engine](https://github.com/TheCherno/Hazel) - architecture inspiration
- **Graphics:** raylib documentation

## Current State

**Commit #1:** ✅ Project setup with raylib, basic window, VSCode integration  
**Commit #2:** ✅ Logger system (thread-safe, colored console, printf-style)  
**Commit #3:** ✅ Time system (DeltaTime, TimeScale, FPS tracking)  
**Commit #4:** ✅ Event system (EventDispatcher, Window/Key/Mouse events)  
**Commit #5:** ✅ Input Manager (polling API, KeyCode/MouseButton enums, platform abstraction)  
**Commit #6:** ✅ Layer System (Layer base class, LayerStack with overlays)  
**Commit #7:** ✅ Application Class (game loop, window management, layer orchestration)  
**Commit #8:** ✅ Sprite System (texture loading, source rect, draw with rotation/scale)  
**Commit #9:** ✅ Animation System (frame-based animation, AnimationController, FPS control)  
**Commit #10:** ✅ Camera System (2D camera, zoom, bounds, coordinate transforms)  
**Commit #11:** ✅ Tilemap System (2D tile grid, tileset loading, passable tiles, culling)
**Commit #12:** ✅ Sprite Renderer (batching, texture sorting, draw call optimization)
**Commit #13:** ✅ A* Pathfinding (path search, heuristic, obstacle avoidance)

## Planned Commits

**Commit #14:** ⏭️ Resource Manager (SKIPPED - will implement later with more context)
**Commit #15:** Unit System (base Unit class, movement along path, sprite rendering)
**Commit #16:** Selection System (mouse drag selection, multi-unit selection, visual feedback)
**Commit #17:** Command System (right-click movement, unit queueing, pathfinding integration)

## Architecture Decisions

### Engine vs Game Separation
```
src/
├── engine/         (Reusable, game-agnostic)
│   ├── logging/    (Logger)
│   ├── core/       (Engine, Time, Application)
│   ├── events/     (Event system)
│   ├── input/      (Input abstraction)
│   ├── graphics/   (Sprite, Animation, Renderer)
│   └── resources/  (ResourceManager)
├── game/           (Warcraft-specific)
│   ├── entities/   (Unit, Building)
│   ├── gameplay/   (Player, Resources, Combat)
│   └── ...
└── main.cpp
```

**Rule:** Engine NEVER includes game code. Game uses engine API.

### Code Style
- **No comments** unless absolutely necessary (self-documenting code)
- **No abbreviations** except universally accepted (ctx, cfg, idx)
- **Meaningful names** over brevity
- **Modern C++17** features preferred
- **Windows-only** (no #ifdef for other platforms)
- **Performance first**, then readability, then maintainability

### Logger Specifics
- Singleton pattern with double-checked locking
- Thread-safe with std::mutex
- Console-only output (no file logging - not needed for dev)
- 4 levels: DEBUG, INFO, WARN, FATAL (not ERROR - conflicts with Windows macro)
- Macros: `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`
- VT100 color codes enabled via Windows API

### Time Specifics
- Singleton pattern with static API
- `GetDeltaTime()` - frame time with TimeScale applied
- `GetUnscaledDeltaTime()` - raw frame time (for UI)
- `GetTime()` - total runtime since start
- `GetFPS()` - current frames per second
- `SetTimeScale(float)` - slow-motion/fast-forward (1.0 = normal)
- Frame-independent movement: `position += speed * Time::GetDeltaTime()`

### Event Specifics
- Event base class with EventType and EventCategory (bitflags)
- EventDispatcher with template-based type dispatch
- Header-only implementation (no .cpp files)
- Macros: `EVENT_CLASS_TYPE`, `EVENT_CLASS_CATEGORY` (reduce boilerplate)
- Event categories: Application, Input, Keyboard, Mouse, MouseButton
- WindowEvents: Close, Resize, Focus, LostFocus
- KeyEvents: Pressed, Released, Typed (with repeat count)
- MouseEvents: Moved, Scrolled, ButtonPressed, ButtonReleased
- `handled` flag stops event propagation

### Input Specifics
- Polling-based API (not event-driven)
- Custom KeyCode enum (A-Z, 0-9, arrows, modifiers)
- Custom MouseButton enum (Left, Right, Middle)
- Custom Vector2 struct (independent from raylib)
- Static methods only (no singleton needed)
- Three states per key: Pressed (single frame), Down (held), Released (single frame)
- Platform abstraction: `KeyCodeToRaylib()`, `MouseButtonToRaylib()` mapping
- Used for gameplay (movement, camera) - events for UI/single actions
- `::` prefix on raylib calls to avoid name conflicts

### Layer Specifics
- Layer base class with virtual methods: `OnAttach()`, `OnDetach()`, `OnUpdate()`, `OnEvent()`
- LayerStack manages layers and overlays in order
- Layers pushed first (bottom), overlays pushed last (top)
- Update order: bottom to top (layers iterate forward)
- Event order: top to bottom (overlays handle events first, reverse iterate)
- `layerInsertIndex` tracks boundary between layers and overlays
- LayerStack owns layer pointers (deletes on destruction)
- Each layer has name for debugging
- Overlays always rendered on top (UI, debug info)
- Event propagation stops if `event.handled = true`

### Application Specifics
- Base class for game applications (engine/core/)
- Singleton pattern with static `Get()` access
- Owns window (raylib::Window*) and LayerStack
- Main game loop in `Run()`: Time::Update(), layer updates, rendering
- `PushLayer()/PushOverlay()` - convenience wrappers for LayerStack
- Game inherits from Application (e.g., `WarcraftGame : public Application`)
- Constructor creates window with width, height, title, target FPS
- Destructor cleans up window and layers
- `GetWindow()` provides access to window for layers

### Sprite Specifics
- Wraps raylib Texture2D with flexible rendering
- `sourceRect` - allows sprite sheets (any rect from texture)
- Multiple constructors: empty, full texture, or custom rect
- `Draw()` with position, rotation, scale parameters
- `Draw()` with origin point for rotation pivot
- `SetTint()` for color modulation
- `GetSize()` returns sprite dimensions
- Auto-unload in destructor (RAII)
- Supports any texture size (units, buildings, terrain)

### Animation Specifics
- `Animation` class - manages frame sequence, FPS, loop mode
- Frame storage as `std::vector<Rectangle>` for sprite sheet coords
- `Update(deltaTime)` - advances animation based on time
- `Play/Stop/Reset` controls
- Loop or once mode (for death animations)
- `GetCurrentFrame()` returns current Rectangle for Sprite
- `AnimationController` - manages multiple named animations
- `Play("idle")` switches animations, auto-resets previous
- Controller updates Sprite's sourceRect automatically
- Used for: unit walk cycles, attack animations, idle breathing

### Camera Specifics
- `Camera2D` class wraps raylib Camera2D for 2D world navigation
- Position and zoom control (clamped 0.1x - 5.0x)
- `Move()` for relative movement, `SetPosition()` for absolute
- `SetBounds()` / `EnableBounds()` - prevent camera from leaving map area
- `Begin()` / `End()` - wrap rendering in camera space
- `ScreenToWorld()` / `WorldToScreen()` - coordinate transformations for mouse clicks
- Bounds clamping in `ClampToBounds()` prevents out-of-map camera
- Used for: map scrolling, following units, minimap clicks

### Tilemap Specifics
- `Tilemap` class manages 2D grid of tiles (terrain)
- Fixed tile size (typically 32x32 for Warcraft 2)
- `LoadTileset()` - loads tileset PNG, supports spacing between tiles
- `LoadFromCSV()` - loads map data exported from Tiled Map Editor
- Supports multi-layer rendering (ground, forest, buildings, etc.)
- 2D vector storage `std::vector<std::vector<Tile>>`
- Each tile has `id` (which sprite) and `passable` flag (for pathfinding)
- `SetTile/GetTile` - modify/query individual tiles
- `TileToWorld/WorldToTile` - coordinate conversions
- `Draw()` with frustum culling - only renders visible tiles (performance)
- Frustum culling adapts to screen size using `GetScreenWidth()/GetScreenHeight()`
- `IsPassable()` - used by A* pathfinding later
- CSV format: comma-separated tile IDs, one row per line (-1 = empty tile)
- Auto-resizes tilemap if CSV dimensions differ from constructor
- Spacing parameter handles pixel gaps between tiles in tileset (e.g., 1px margin)
- Tile IDs match Tiled Map Editor directly (no conversion needed)
- Assets organized: `assets/maps/winter/` (tileset.png, ground.csv, forest.csv)
- **Note:** Each Tilemap loads its own tileset copy (no texture sharing yet). Will be optimized with Resource Manager

### SpriteRenderer Specifics
- Batching system for optimizing sprite rendering
- `Begin()` starts batch collection, `End()` flushes to GPU
- `DrawSprite()` adds sprites to batch buffer (not drawn immediately)
- Sorts sprites by texture ID to minimize texture switches (draw call optimization)
- Reduces GPU state changes from hundreds to just a few
- Provides statistics: batch count (total sprites) and draw call count (GPU calls)
- Used by Tilemap for efficient tile rendering
- Performance benefit: ~800 tiles rendered with minimal draw calls instead of 800 separate GPU calls
- **Note:** Currently shows 2 draw calls for ground+forest layers (same PNG loaded twice with different texture IDs). Will be fixed with Resource Manager

### Pathfinder Specifics
- A* pathfinding algorithm implementation
- `FindPath()` static method takes start, goal, and passability check function
- Returns `std::vector<Vector2>` of tile coordinates forming the path
- Uses Manhattan distance heuristic (optimized for 4-directional movement)
- Open/Closed lists for node management
- `PathNode` structure: x, y, g (cost from start), h (heuristic to goal), f (g+h), parent
- 4-directional movement (N, S, E, W) - no diagonals (matches Warcraft 2)
- Integrates with `Tilemap::IsPassable()` for obstacle detection
- Handles edge cases: unreachable goals, invalid start/goal, same start/goal
- Memory management: properly cleans up allocated nodes after search

## Known Issues & Future Improvements

### Texture Duplication Issue
**Problem:** Each `Tilemap` loads its own copy of the same tileset into GPU memory.
- `groundLayer->LoadTileset("tileset.png")` → Texture ID 10
- `forestLayer->LoadTileset("tileset.png")` → Texture ID 20 (duplicate!)
- Result: 2x memory usage, 2 draw calls instead of 1

**Current Impact:**
- Minor performance hit (2 draw calls vs 1 for tilemap)
- Wasted GPU memory (4 MB vs 2 MB for winter tileset)
- Acceptable for current demo stage

**Solution (Future Commit):** Resource Manager system
- Centralized texture loading with path-based caching
- `ResourceManager::GetTexture("tileset.png")` returns same instance
- All tilemaps share single GPU texture → 1 draw call
- Also needed for: unit sprites, building sprites, UI textures, effects
- Will become critical when adding multiple tilesets, units, buildings

## Build System

**Versions:**
- CMake 3.14+
- raylib 5.0 (FetchContent)
- raylib-cpp v5.0.1 (FetchContent)
- C++17
- MSYS2 MINGW64 g++

**Build:**
```bash
./build.bat           # Clean build
cmake --build build   # Quick rebuild
```

## Commit Strategy

**One commit = one working feature**

Each commit should:
- Compile without errors
- Run without crashes
- Add exactly ONE system/feature
- Have clear commit message format

**Format:**
```
type: brief description

- Detailed point 1
- Detailed point 2
```

**Types:** feat, refactor, perf, fix

## Development Plan (30 commits)

### Phase 1: Core Infrastructure (Commits 1-7)
1. ✅ Project setup
2. ✅ Logger system
3. ✅ Time system
4. ✅ Event system
5. ✅ Input manager
6. ✅ Layer system
7. ✅ Application class

### Phase 2: Graphics (Commits 8-12)
8. ✅ Sprite system
9. ✅ Animation system
10. ✅ Camera system
11. ✅ Tilemap system
12. ✅ Sprite renderer

### Phase 3: Game Logic (Commits 13-17)
13. ✅ A* pathfinding
14. ⏭️ Resource Manager (skipped)
15. Unit system
16. Selection system
17. Movement commands

### Phase 4: UI (Commits 18-20)
18. UI framework
19. HUD (resources, portrait)
20. Minimap

### Phase 5: Gameplay (Commits 21-25)
21. Resource system (gold, wood)
22. Building system
23. Unit production
24. Combat system
25. Unit AI (FSM)

### Phase 6: Polish (Commits 26-31)
26. Fog of war
27. Audio system
28. Enemy AI
29. Optimization (quadtree)
30. Particle effects
31. Game states (menu, pause)

## Assets

**Spritesheet format:**
- Orc worker spritesheet available
- Multiple animations per sheet (idle, walk, attack, gather, death)
- 5-8 directional sprites
- Will need sprite parsing system

## Important Notes

- **Windows-only**: No cross-platform abstractions needed
- **raylib-cpp**: Header-only wrapper, must be in includes
- **MSVC issues**: We compile with g++ (MSYS2), not MSVC
- **Singleton debate**: Using traditional pointer-based singleton for control
- **No file logs**: Console output only, simpler for development

## When Starting New Chat

1. Read this file first
2. Check current commit state in git log
3. Review recent file changes
4. Continue from "Next Steps" section
5. Follow architecture decisions strictly

## Questions to Ask

If unclear about direction:
- "What's the next commit we're working on?"
- "Should this go in engine/ or game/?"
- "Is this following Hazel's architecture pattern?"
- "Does this need thread-safety?"
