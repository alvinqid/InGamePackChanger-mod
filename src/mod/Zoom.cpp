#include "mod/Zoom.hpp"

static bool isEnabled()
{
    auto& keyStates = Keyboard::_states();
    return keyStates[Keyboard::C] != 0;
}

namespace alvinqid {

    LL_AUTO_TYPE_INSTANCE_HOOK(
        LevelRendererPlayerFov,
        HookPriority::Normal,
        LevelRendererPlayer,
        &LevelRendererPlayer::getFov,
        float,
        float a, bool b
    ) {
        float currentFov = origin(a,b);
        if(currentFov == 70.0f && !b) return currentFov;
        if(isEnabled()) return 10.0f;
    
        return currentFov;
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        BaseOptionsSensi,
        HookPriority::Normal,
        BaseOptions,
        &BaseOptions::$getSensitivity,
        float,
        InputMode inputMode
    ) {
        float currentSensitivityorigin(inputMode);
        float dampen = (100.0f - 90.0f) / 100.0f;
        float targetSensitivity = currentSensitivity * dampen;
        
        if(isEnabled()) return targetSensitivity;
    
        return currentSensitivity;
    }
    
    bool Zoom::load() {
        getSelf().getLogger().debug("Loading...");
        return true;
    }
    
    bool Zoom::enable() {
        getSelf().getLogger().debug("Enabling...");
        return true;
    }
    
    bool Zoom::disable() {
        getSelf().getLogger().debug("Disabling...");
        return true;
    }

} // namespace alvinqid

LL_REGISTER_MOD(alvinqid::Zoom, alvinqid::Zoom::getInstance());