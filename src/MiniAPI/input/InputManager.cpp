#include <utility>

#include "MiniAPI/input/InputManager.hpp"
#include "MiniAPI/input/InputAction.hpp"
#include "mc/client/game/IClientInstance.h"
#include "mc/client/input/VanillaClientInputMappingFactory.h"

MiniAPI::InputManager::InputManager(Options *opt) {
    mOptions = opt;
}

MiniAPI::InputManager::~InputManager() {
    for (auto& action : mCustomInputs) {
        for (auto& mapping : mOptions->mKeyboardRemappings) {
            auto newEnd = std::remove_if(
                mapping->mKeymappings.begin(),
                mapping->mKeymappings.end(),
                [&action](const Keymapping& keymapping)
            {
                    return keymapping.mAction == std::string("key." + action.mActionName);
            });
            mapping->mKeymappings.erase(newEnd, mapping->mKeymappings.end());
        }
    }

    mCustomInputs.clear();
    mActions.clear();
}

MiniAPI::InputAction& MiniAPI::InputManager::RegisterNewInput(const std::string& actionName, std::vector<int> defaultKeys, bool allowRemapping, KeybindContext context)
{
    io::Logger& logger = alvinqid::MiniAPI::getInstance().getSelf().getLogger();

    if(context == KeybindContext::None) {
        logger.error("Cannot register a keybind with context None");
        return;
    }
    if(actionName.find("button.") == 0 && actionName.find("key.") == 0) {
        logger.error("Action name does not need to start with button. or key.");
        return;
    }
    if(mOptions == nullptr) {
        logger.error("MiniAPI::InputManager->mOptions was nullptr. Ensure that RegisterNewInput is being called in the RegisterInputs event.");
        return;
    }

    uint32_t hash = StringToNameId("button." + actionName);

    auto it = mActions.find(hash);
    if (it != mActions.end()) {
        return *(it->second);
    }

    for (auto& layout : mOptions->mKeyboardRemappings) {
        Keymapping keymapping("key." + actionName, defaultKeys, allowRemapping);
        layout->mKeymappings.emplace_back(keymapping);
        layout->mDefaultMappings.emplace_back(keymapping);
    }

    mCustomInputs.emplace_back(actionName, std::move(defaultKeys), allowRemapping, context);

    auto action = std::make_unique<MiniAPI::InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

MiniAPI::InputAction& MiniAPI::InputManager::GetVanillaInput(const std::string& actionName)
{
    Assert(actionName.find("button.") != 0 && actionName.find("key.") != 0, "Action name does not need to start with button. or key.");
    uint32_t hash = StringToNameId("button." + actionName);

    auto it = mActions.find(hash);
    if (it != mActions.end()) {
        return *(it->second);
    }

    auto action = std::make_unique<MiniAPI::InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

MiniAPI::InputPassthrough MiniAPI::InputManager::_handleButtonEvent(InputHandler* handler, const ButtonEventData& button, FocusImpact focus, IClientInstance& client, int controllerId) const
{
    auto it = mActions.find(button.id);
    if (it == mActions.end()) return InputPassthrough::Passthrough;

    const MiniAPI::InputAction& action = *it->second.get();
    return action._onButtonStateChange(button.state, focus, client.asInstance());
}

void MiniAPI::InputManager::createKeyboardAndMouseBinding(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, const std::string* buttonName, const std::string* keyName, FocusImpact impact) {
    Keymapping& mapping = *inputs->mKeyboardRemappingLayout->getKeymappingByAction(*keyName);

	for(int key : mapping.mKeys) {
		if (!mapping.isAssigned()) continue;
        if (mapping.isAltKey()) {
            mouse->buttonBindings.emplace_back(buttonName, key);
		}
        else {
            keyboard->keyBindings.emplace_back(buttonName, key, impact);
        }
	}
}

void MiniAPI::InputManager::_registerKeyboardInputs(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, MiniAPI::KeybindContext context)
{
    for (auto& input : mCustomInputs) {
        if ((input.mContexts & context) == MiniAPI::KeybindContext::None) {
            continue;
        }

        std::string keyName = "key." + input.mActionName;
        std::string buttonName = "button." + input.mActionName;
        createKeyboardAndMouseBinding(inputs, keyboard, mouse, &buttonName, &keyName);
    }
}
