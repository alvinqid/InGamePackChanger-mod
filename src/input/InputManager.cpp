#include <utility>

#include "input/InputManager.hpp"
#include "input/InputAction.hpp"
#include "mc/client/input/VanillaClientInputMappingFactory.h"
#include "mc/client/game/IClientInstance.h"

InputManager::InputManager() {}

/*InputManager::~InputManager() {
    for (auto& action : mCustomInputs) {
        auto mapping = alvinqid::Zoom::getInstance().getProperty().keyboardLayout;

        auto& vec = mapping->mKeymappings.get();

        auto newEnd = std::remove_if(
            vec.begin(),
            vec.end(),
            [&action](const Keymapping& keymapping)
            {
                return keymapping.mAction == ("key." + action.mActionName);
            });

        vec.erase(newEnd, vec.end());
    }

    mCustomInputs.clear();
    mActions.clear();
}*/

InputAction& InputManager::registerNewInput(const std::string& actionName, std::vector<int> const& defaultKeys, bool allowRemapping, KeybindContext context)
{
/*
    Assert(context != KeybindContext::None, "Cannot register a keybind with context None");
    Assert(actionName.find("button.") != 0 && actionName.find("key.") != 0, "Action name does not need to start with button. or key.");
    Options* options = GetClientCtx().mOptions;
    Assert(options != nullptr, "InputManager->mOptions was nullptr. Ensure that RegisterNewInput is being called in the RegisterInputs event.");
*/
    uint32_t hash = StringToNameId("button." + actionName);

    // Look for an existing listner with the same name.
    auto it = mActions.find(hash);
    if (it != mActions.end()) {
        return *(it->second);
    }

    // Add it to options which allows for it to be remapped, and is necessary for listeners
    auto layout = alvinqid::Zoom::getInstance().getProperty().keyboardLayout;
    Keymapping keymapping("key." + actionName, defaultKeys);
    layout->mKeymappings.get().emplace_back(keymapping);
    layout->mDefaultMappings.get().emplace_back(keymapping);


    // Add to custom inputs so it can be registered in other places by the runtime
    mCustomInputs.emplace_back(actionName, std::move(defaultKeys), allowRemapping, context);

    auto action = std::make_unique<InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

InputAction& InputManager::getVanillaInput(const std::string& actionName)
{
    //Assert(actionName.find("button.") != 0 && actionName.find("key.") != 0, "Action name does not need to start with button. or key.");
    uint32_t hash = StringToNameId("button." + actionName);

    // Look for an existing listner with the same name.
    auto it = mActions.find(hash);
    if (it != mActions.end()) {
        return *(it->second);
    }

    auto action = std::make_unique<InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

InputPassthrough InputManager::_handleButtonEvent(InputHandler* handler, const ButtonEventData& button, FocusImpact focus, IClientInstance& client, int controllerId) const
{
    auto it = mActions.find(button.id);
    if (it == mActions.end()) return InputPassthrough::Passthrough;

    const InputAction& action = *it->second.get();
    return action._onButtonStateChange(button.state, focus, *alvinqid::Zoom::getInstance().getProperty().client);
}

void InputManager::_registerKeyboardInputs(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, KeybindContext context)
{
    for (auto& input : mCustomInputs) {
        if ((input.mContexts & context) == KeybindContext::None) {
            continue;
        }

        std::string keyName = "key." + input.mActionName;
        std::string buttonName = "button." + input.mActionName;
        //inputs->createKeyboardAndMouseBinding(keyboard, mouse, &buttonName, &keyName);
        Keymapping mapping = *alvinqid::Zoom::getInstance().getProperty().keyboardLayout->getKeymappingByAction(keyName);

    	for(int key : mapping.mKeys) {
    		if (!mapping.isAssigned()) continue;
    
            if (mapping.isAltKey()) {
                mouse->buttonBindings.get().emplace_back(buttonName, key);
    		}
            else {
                keyboard->keyBindings.get().emplace_back(buttonName, key, FocusImpact::Neutral);
            }
    	}
    }
}