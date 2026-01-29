#pragma once
#include "Event.h"

namespace Engine {

class KeyEvent : public Event {
public:
    int GetKeyCode() const { return keyCode; }
    
    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    
protected:
    KeyEvent(int keyCode) : keyCode(keyCode) {}
    
    int keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keyCode, int repeatCount)
        : KeyEvent(keyCode), repeatCount(repeatCount) {}
    
    int GetRepeatCount() const { return repeatCount; }
    
    std::string ToString() const override {
        return std::string("KeyPressedEvent: ") + std::to_string(keyCode) + " (" + std::to_string(repeatCount) + " repeats)";
    }
    
    EVENT_CLASS_TYPE(KeyPressed)
    
private:
    int repeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
    
    std::string ToString() const override {
        return std::string("KeyReleasedEvent: ") + std::to_string(keyCode);
    }
    
    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}
    
    std::string ToString() const override {
        return std::string("KeyTypedEvent: ") + std::to_string(keyCode);
    }
    
    EVENT_CLASS_TYPE(KeyTyped)
};

}
