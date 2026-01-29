#pragma once
#include <string>
#include "engine/events/Event.h"

namespace Engine {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(Event& event) {}

    const std::string& GetName() const { return name; }

protected:
    std::string name;
};

}
