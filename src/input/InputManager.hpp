#pragma once
#include <vector>
#include <map>
#include "input/InputAction.hpp"
#include "mc/client/options/Options.h"
#include "mc/deps/input/InputHandler.h"
#include "mc/client/input/ClientInputHandler.h"
#include "mc/client/input/VanillaClientInputMappingFactory.h"
#include "mc/deps/input/KeyboardInputMapping.h"
#include "mc/deps/input/MouseInputMapping.h"
#include "mod/Zoom.hpp"
#include "mc/client/input/KeyboardRemappingLayout.h"
#include "mc/deps/input/ButtonEventData.h"

//class AmethystContext;

//namespace Amethyst {
class InputManager {
public:
    InputManager(/*AmethystContext* amethyst*/);
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;
    ~InputManager();

    InputAction& registerNewInput(const std::string& actionName, std::vector<int> const& defaultKeys, bool allowRemapping = true, KeybindContext context = KeybindContext::Gameplay);
    InputAction& getVanillaInput(const std::string& actionName);

private:
    std::unordered_map<uint32_t, std::unique_ptr<InputAction>> mActions;
    std::vector<InputActionOptions> mCustomInputs;
/*
private:
    AmethystContext* mAmethyst;
    friend class AmethystRuntime;
*/
public:
    InputPassthrough _handleButtonEvent(InputHandler* handler, const ButtonEventData& button, FocusImpact focus, IClientInstance& client, int controllerId) const;
    void _registerKeyboardInputs(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, KeybindContext context);
};

//}