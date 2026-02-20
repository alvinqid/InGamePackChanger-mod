#pragma once

#include <vector>
#include <utility>
#include <map>

#include "input/InputAction.hpp"
#include "mc/client/game/ClientInstance.h"
#include "mc/client/game/IClientInstance.h"
#include "mc/client/options/Options.h"
#include "mc/deps/input/InputHandler.h"
#include "mc/deps/input/KeyboardInputMapping.h"
#include "mc/deps/input/MouseInputMapping.h"
#include "mc/client/input/Keymapping.h"
#include "mc/client/input/ClientInputHandler.h"
#include "mc/client/input/VanillaClientInputMappingFactory.h"

namespace Amethyst {
class InputManager {
public:
    Options* mOptions;
    
    InputManager(AmethystContext* amethyst);
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;
    ~InputManager();

    InputAction& RegisterNewInput(const std::string& actionName, std::vector<int> defaultKeys, bool allowRemapping = true, KeybindContext context = KeybindContext::Gameplay);
    InputAction& GetVanillaInput(const std::string& actionName);

    void createKeyboardAndMouseBinding(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, const std::string* buttonName, const std::string* keyName, FocusImpact impact = FocusImpact::Neutral);
private:
    std::unordered_map<uint32_t, std::unique_ptr<InputAction>> mActions;
    std::vector<InputActionOptions> mCustomInputs;

public:
    InputPassthrough _handleButtonEvent(InputHandler* handler, const ButtonEventData& button, FocusImpact focus, IClientInstance& client, int controllerId) const;
    void _registerKeyboardInputs(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, Amethyst::KeybindContext context);
};

}