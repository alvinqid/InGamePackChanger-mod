#pragma once
#include <vector>
#include <map>
#include "mc/client/options/Options.h"
#include "mc/client/options/IOptions.h"
#include "mc/client/input/ClientInputHandler.h"
#include "mc/deps/input/InputHandler.h"
#include "MiniAPI/input/InputAction.hpp"
#include "mod/MiniAPI.hpp"
#include "mc/client/game/ClientInstance.h"

class VanillaClientInputMappingFactory;
class KeyboardInputMapping;
class MouseInputMapping;

namespace MiniAPI {
class InputManager {
public:
    InputManager(Options* opt);
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

private:
    Options* mOptions;

public:
    InputPassthrough _handleButtonEvent(InputHandler* handler, const ButtonEventData& button, FocusImpact focus, IClientInstance& client, int controllerId) const;
    void _registerKeyboardInputs(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, MiniAPI::KeybindContext context);
};

}