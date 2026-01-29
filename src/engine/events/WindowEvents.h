#pragma once
#include "Event.h"

namespace Engine {

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;
    
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(int width, int height)
        : width(width), height(height) {}
    
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    
    std::string ToString() const override {
        return std::string("WindowResizeEvent: ") + std::to_string(width) + "x" + std::to_string(height);
    }
    
    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    
private:
    int width;
    int height;
};

class WindowFocusEvent : public Event {
public:
    WindowFocusEvent() = default;
    
    EVENT_CLASS_TYPE(WindowFocus)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowLostFocusEvent : public Event {
public:
    WindowLostFocusEvent() = default;
    
    EVENT_CLASS_TYPE(WindowLostFocus)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

}
