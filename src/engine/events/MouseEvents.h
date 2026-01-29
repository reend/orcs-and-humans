#pragma once
#include "Event.h"

namespace Engine {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}
    
    float GetX() const { return mouseX; }
    float GetY() const { return mouseY; }
    
    std::string ToString() const override {
        return std::string("MouseMovedEvent: ") + std::to_string(mouseX) + ", " + std::to_string(mouseY);
    }
    
    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
private:
    float mouseX;
    float mouseY;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : xOffset(xOffset), yOffset(yOffset) {}
    
    float GetXOffset() const { return xOffset; }
    float GetYOffset() const { return yOffset; }
    
    std::string ToString() const override {
        return std::string("MouseScrolledEvent: ") + std::to_string(xOffset) + ", " + std::to_string(yOffset);
    }
    
    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
private:
    float xOffset;
    float yOffset;
};

class MouseButtonEvent : public Event {
public:
    int GetMouseButton() const { return button; }
    
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    
protected:
    MouseButtonEvent(int button) : button(button) {}
    
    int button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
    
    std::string ToString() const override {
        return std::string("MouseButtonPressedEvent: ") + std::to_string(button);
    }
    
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
    
    std::string ToString() const override {
        return std::string("MouseButtonReleasedEvent: ") + std::to_string(button);
    }
    
    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}
