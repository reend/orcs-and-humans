#pragma once

namespace Engine {

enum class KeyCode {
    Unknown = 0,
    Space, Enter, Escape, Tab, Backspace,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Left, Right, Up, Down,
    LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt
};

enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2
};

struct Vector2 {
    float x;
    float y;
};

class Input {
public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsKeyDown(KeyCode key);
    static bool IsKeyReleased(KeyCode key);
    
    static bool IsMouseButtonPressed(MouseButton button);
    static bool IsMouseButtonDown(MouseButton button);
    static bool IsMouseButtonReleased(MouseButton button);
    
    static Vector2 GetMousePosition();
    static float GetMouseWheelMove();

private:
    static int KeyCodeToRaylib(KeyCode key);
    static int MouseButtonToRaylib(MouseButton button);
};

}
