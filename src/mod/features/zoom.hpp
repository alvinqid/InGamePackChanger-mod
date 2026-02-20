#pragma once
#include "mod/api/InputManager.hpp"

#include <chrono>

namespace alvinqid {

    class Features {
    public:
    
        void setEnabled(bool state);
        
        float getSensitivity(float currentSensitivity);
        
        float getFov(float currentFov, bool applyEffects);
        
        void RegisterCustomInputs(InputManager inputManager);
    
    private:
        bool enabled = false;
    }
} // namespace alvinqid