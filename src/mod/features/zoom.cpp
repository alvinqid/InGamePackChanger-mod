#include "mod/features/zoom.hpp"

namespace alvinqid {

    void Features::setEnabled(bool state) {
        enabled = state;
    }

    void Features::RegisterCustomInputs(InputManager inputManager) {
        InputAction& zoomKeybind =
            inputManager.registerNewInput("zoom", {'C'}, true, KeybindContext::Gameplay);
    
        zoomKeybind.addButtonDownHandler([](FocusImpact focus) {
            setEnabled(true);
            return InputPassthrough::Consume;
        });
    
        zoomKeybind.addButtonUpHandler([](FocusImpact focus) {
            setEnabled(false);
            return InputPassthrough::Consume;
        });
    }
    
    float Features::getFov(float currentFov, bool applyEffects) {
        if(currentFov == 70.0f && !applyEffects) return currentFov;
        if(enabled) return 10.0f;
    
        return currentFov;
    }
    
    float Features::getSensitivity(float currentSensitivity) {
        float dampen = (100.0f - 90.0f) / 100.0f;
        float targetSensitivity = currentSensitivity * dampen;
        
        if(enabled) return targetSensitivity;
    
        return currentSensitivity;
    }
} // namespace alvinqid