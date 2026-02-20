#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include "mod/InputAction.hpp"

#include "mc/client/options/Options.h"
#include "mc/deps/input/InputHandler.h"
#include "mc/deps/input/KeyboardInputMapping.h"
#include "mc/deps/input/MouseInputMapping.h"
#include "mc/client/input/Keymapping.h"
#include "mc/client/input/VanillaClientInputMappingFactory.h"

class IClientInstance;
class ClientInstance;
class RemappedLayout;

class InputManager {
public:
    InputManager(Options* opts, RemappedLayout* layout);

    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    InputAction& registerNewInput(
        const std::string& actionName,
        std::vector<int> defaultKeys,
        bool allowRemapping = true,
        KeybindContext context = KeybindContext::Gameplay
    );

    InputAction& getVanillaInput(const std::string& actionName);

    void createKeyboardAndMouseBinding(
        KeyboardInputMapping* keyboard,
        MouseInputMapping* mouse,
        const std::string* buttonName,
        const std::string* keyName,
        FocusImpact impact = FocusImpact::Neutral
    );

    InputPassthrough _handleButtonEvent(
        InputHandler* handler,
        const ButtonEventData& button,
        FocusImpact focus,
        IClientInstance& client,
        int controllerId
    ) const;

    void _registerKeyboardInputs(
        VanillaClientInputMappingFactory* inputs,
        KeyboardInputMapping* keyboard,
        MouseInputMapping* mouse,
        KeybindContext context
    );

private:
    Options* mOptions{};
    RemappedLayout* mLayout{};

    std::unordered_map<uint32_t, std::unique_ptr<InputAction>> mActions;
    std::vector<InputActionOptions> mCustomInputs;
};