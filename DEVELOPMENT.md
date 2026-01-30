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
10. Camera system
11. Map/tilemap system
12. Sprite renderer

### Phase 3: Game Logic (Commits 13-17)
13. A* pathfinding
14. Unit system
15. Selection system
16. Movement commands
17. Multiple units

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
