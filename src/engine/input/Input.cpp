#include "Input.h"
#include "raylib.h"

namespace Engine {

bool Input::IsKeyPressed(KeyCode key) {
    return ::IsKeyPressed(KeyCodeToRaylib(key));
}

bool Input::IsKeyDown(KeyCode key) {
    return ::IsKeyDown(KeyCodeToRaylib(key));
}

bool Input::IsKeyReleased(KeyCode key) {
    return ::IsKeyReleased(KeyCodeToRaylib(key));
}

bool Input::IsMouseButtonPressed(MouseButton button) {
    return ::IsMouseButtonPressed(MouseButtonToRaylib(button));
}

bool Input::IsMouseButtonDown(MouseButton button) {
    return ::IsMouseButtonDown(MouseButtonToRaylib(button));
}

bool Input::IsMouseButtonReleased(MouseButton button) {
    return ::IsMouseButtonReleased(MouseButtonToRaylib(button));
}

Vector2 Input::GetMousePosition() {
    ::Vector2 pos = ::GetMousePosition();
    return {pos.x, pos.y};
}

float Input::GetMouseWheelMove() {
    return ::GetMouseWheelMove();
}

int Input::KeyCodeToRaylib(KeyCode key) {
    switch (key) {
        case KeyCode::Space: return KEY_SPACE;
        case KeyCode::Enter: return KEY_ENTER;
        case KeyCode::Escape: return KEY_ESCAPE;
        case KeyCode::Tab: return KEY_TAB;
        case KeyCode::Backspace: return KEY_BACKSPACE;
        
        case KeyCode::A: return KEY_A;
        case KeyCode::B: return KEY_B;
        case KeyCode::C: return KEY_C;
        case KeyCode::D: return KEY_D;
        case KeyCode::E: return KEY_E;
        case KeyCode::F: return KEY_F;
        case KeyCode::G: return KEY_G;
        case KeyCode::H: return KEY_H;
        case KeyCode::I: return KEY_I;
        case KeyCode::J: return KEY_J;
        case KeyCode::K: return KEY_K;
        case KeyCode::L: return KEY_L;
        case KeyCode::M: return KEY_M;
        case KeyCode::N: return KEY_N;
        case KeyCode::O: return KEY_O;
        case KeyCode::P: return KEY_P;
        case KeyCode::Q: return KEY_Q;
        case KeyCode::R: return KEY_R;
        case KeyCode::S: return KEY_S;
        case KeyCode::T: return KEY_T;
        case KeyCode::U: return KEY_U;
        case KeyCode::V: return KEY_V;
        case KeyCode::W: return KEY_W;
        case KeyCode::X: return KEY_X;
        case KeyCode::Y: return KEY_Y;
        case KeyCode::Z: return KEY_Z;
        
        case KeyCode::Num0: return KEY_ZERO;
        case KeyCode::Num1: return KEY_ONE;
        case KeyCode::Num2: return KEY_TWO;
        case KeyCode::Num3: return KEY_THREE;
        case KeyCode::Num4: return KEY_FOUR;
        case KeyCode::Num5: return KEY_FIVE;
        case KeyCode::Num6: return KEY_SIX;
        case KeyCode::Num7: return KEY_SEVEN;
        case KeyCode::Num8: return KEY_EIGHT;
        case KeyCode::Num9: return KEY_NINE;
        
        case KeyCode::Left: return KEY_LEFT;
        case KeyCode::Right: return KEY_RIGHT;
        case KeyCode::Up: return KEY_UP;
        case KeyCode::Down: return KEY_DOWN;
        
        case KeyCode::LeftShift: return KEY_LEFT_SHIFT;
        case KeyCode::RightShift: return KEY_RIGHT_SHIFT;
        case KeyCode::LeftCtrl: return KEY_LEFT_CONTROL;
        case KeyCode::RightCtrl: return KEY_RIGHT_CONTROL;
        case KeyCode::LeftAlt: return KEY_LEFT_ALT;
        case KeyCode::RightAlt: return KEY_RIGHT_ALT;
        
        default: return KEY_NULL;
    }
}

int Input::MouseButtonToRaylib(MouseButton button) {
    switch (button) {
        case MouseButton::Left: return MOUSE_LEFT_BUTTON;
        case MouseButton::Right: return MOUSE_RIGHT_BUTTON;
        case MouseButton::Middle: return MOUSE_MIDDLE_BUTTON;
        default: return MOUSE_LEFT_BUTTON;
    }
}

}
