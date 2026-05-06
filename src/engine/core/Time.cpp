#include "Time.h"
#include "raylib.h"

namespace Engine {

Time::Time()
    : deltaTime(0.0f)
    , timeScale(1.0f)
    , totalTime(0.0) {
}

Time& Time::Get() {
    static Time instance;
    return instance;
}

void Time::Update() {
    Time& t = Get();
    t.deltaTime = ::GetFrameTime();
    t.totalTime = ::GetTime();
}

float Time::GetDeltaTime() {
    Time& t = Get();
    return t.deltaTime * t.timeScale;
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
