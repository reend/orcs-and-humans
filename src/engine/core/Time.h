#pragma once

namespace Engine {

class Time {
private:
    static Time* instance;
    
    float deltaTime;
    float timeScale;
    double totalTime;
    
    Time();
    ~Time() = default;

public:
    static Time& Get();
    
    static void Update();
    
    static float GetDeltaTime();
    static float GetUnscaledDeltaTime();
    static double GetTime();
    static int GetFPS();
    
    static void SetTimeScale(float scale);
    static float GetTimeScale();
};

}
