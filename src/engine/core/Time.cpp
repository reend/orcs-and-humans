#include "Time.h"
#include "raylib.h"

namespace Engine {

Time* Time::instance = nullptr;

Time::Time() 
    : deltaTime(0.0f)
    , timeScale(1.0f)
    , totalTime(0.0) {
}

Time& Time::Get() {
    if (!instance) {
        instance = new Time();
    }
    return *instance;
}

void Time::Update() {
    Get().deltaTime = ::GetFrameTime();
    Get().totalTime = ::GetTime();
}

float Time::GetDeltaTime() {
    return Get().deltaTime * Get().timeScale;
}

float Time::GetUnscaledDeltaTime() {
    return Get().deltaTime;
}

double Time::GetTime() {
    return Get().totalTime;
}

int Time::GetFPS() {
    return ::GetFPS();
}

void Time::SetTimeScale(float scale) {
    Get().timeScale = scale;
}

float Time::GetTimeScale() {
    return Get().timeScale;
}

}
