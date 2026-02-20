#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include "mod/api/InputAction.hpp"

#include "mc/client/options/Options.h"
#include "mc/deps/input/InputHandler.h"
#include "mc/deps/input/KeyboardInputMapping.h"
#include "mc/deps/input/MouseInputMapping.h"
#include "mc/client/input/Keymapping.h"
#include "mc/client/input/RemappedLayout.h"
#include "mc/client/input/VanillaClientInputMappingFactory.h"
#include "mc/client/game/IClientInstance.h"
#include "mc/client/game/ClientInstance.h"

class InputManager {
public:
    InputManager(alvinqid::Zoom zoom);

    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    InputAction& registerNewInput(
        const std::string& actionName,
        std::vector<int> const& defaultKeys,
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
    alvinqid::Zoom mMod;

    std::unordered_map<uint32_t, std::unique_ptr<InputAction>> mActions;
    std::vector<InputActionOptions> mCustomInputs;
};