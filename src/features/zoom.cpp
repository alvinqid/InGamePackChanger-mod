#include "features/zoom.hpp"

static bool enabled = false;

void setEnabled(bool state) {
    enabled = state;
}

float LevelRendererPlayer_getFov(float currentFov, bool applyEffects) {
    /*static std::string zoomType = configManager->getZoomType();
    static float targetFov = configManager->getTargetFov();
    static float duration = configManager->getDuration();*/

    if(currentFov == 70.0f && !applyEffects) return currentFov;

    if(enabled) return 10.0f;

    /*if(zoomType == "gradual") {
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - enabledStart;

        float time = std::clamp(deltaTime.count(), 0.0f, duration);
        float rate = (currentFov - targetFov) / duration;

        if(enabled)
            return currentFov - (time * rate);
        else return targetFov + (time * rate);
    } else if(zoomType == "instant" && enabled) return targetFov;*/
    
    return currentFov;
}

float BaseOptions_getSensitivity(float currentSensitivity, unsigned int inputMode) {
    static float dampen = (100.0f - 90.0f/*configManager->getSensitivityDampen())*/ / 100.0f;

    //static std::string zoomType = configManager->getZoomType();
    //static float duration = configManager->getDuration();

    float targetSensitivity = currentSensitivity * dampen;
    
    if(enabled) return targetSensitivity;

    /*if(zoomType == "gradual") {
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - enabledStart;

        float time = std::clamp(deltaTime.count(), 0.0f, duration);
        float rate = (currentSensitivity - targetSensitivity) / duration;

        if(enabled)
            return (time * -rate) + currentSensitivity;
        else return time * rate;
    } else if(zoomType == "instant" && enabled) return targetSensitivity;*/

    return currentSensitivity;
}