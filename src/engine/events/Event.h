#pragma once
#include <string>
#include <functional>

namespace Engine {

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication = 1 << 0,
    EventCategoryInput = 1 << 1,
    EventCategoryKeyboard = 1 << 2,
    EventCategoryMouse = 1 << 3,
    EventCategoryMouseButton = 1 << 4
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
                               virtual EventType GetEventType() const override { return GetStaticType(); } \
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event {
public:
    virtual ~Event() = default;
    
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }
    
    bool IsInCategory(EventCategory category) const {
        return GetCategoryFlags() & category;
    }
    
    bool handled = false;
};

class EventDispatcher {
public:
    EventDispatcher(Event& event) : event(event) {}
    
    template<typename T, typename F>
    bool Dispatch(const F& func) {
        if (event.GetEventType() == T::GetStaticType()) {
            event.handled |= func(static_cast<T&>(event));
            return true;
        }
        return false;
    }
    
private:
    Event& event;
};

}
