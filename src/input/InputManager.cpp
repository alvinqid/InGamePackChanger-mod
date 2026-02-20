#include "input/InputManager.hpp"

Amethyst::InputManager::InputManager(Options *opts) {
    mOptions = opts;
}

Amethyst::InputAction& Amethyst::InputManager::RegisterNewInput(const std::string& actionName, std::vector<int> defaultKeys, bool allowRemapping, KeybindContext context)
{
    //Assert(context != KeybindContext::None, "Cannot register a keybind with context None");
    //Assert(actionName.find("button.") != 0 && actionName.find("key.") != 0, "Action name does not need to start with button. or key.");
    Options* options = mOptions;
    //Assert(options != nullptr, "Amethyst::InputManager->mOptions was nullptr. Ensure that RegisterNewInput is being called in the RegisterInputs event.");

    uint32_t hash = StringToNameId("button." + actionName);

    // Look for an existing listner with the same name.
    auto it = mActions.find(hash);
    if (it != mActions.end()) {
        return *(it->second);
    }

    // Add it to options which allows for it to be remapped, and is necessary for listeners
    for (auto& layout : options->mKeyboardRemappings.get()) {
        Keymapping keymapping("key." + actionName, defaultKeys, allowRemapping);
        layout->mKeymappings.emplace_back(keymapping);
        layout->mDefaultMappings.emplace_back(keymapping);
    }

    // Add to custom inputs so it can be registered in other places by the runtime
    mCustomInputs.emplace_back(actionName, std::move(defaultKeys), allowRemapping, context);

    auto action = std::make_unique<Amethyst::InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

Amethyst::InputAction& Amethyst::InputManager::GetVanillaInput(const std::string& actionName)
{
    //Assert(actionName.find("button.") != 0 && actionName.find("key.") != 0, "Action name does not need to start with button. or key.");
    uint32_t hash = StringToNameId("button." + actionName);

    // Look for an existing listner with the same name.
    auto it = mActions.find(hash);
    if (it != mActions.end()) {
        return *(it->second);
    }

    auto action = std::make_unique<Amethyst::InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

Amethyst::InputPassthrough Amethyst::InputManager::_handleButtonEvent(InputHandler* handler, const ButtonEventData& button, FocusImpact focus, IClientInstance& client, int controllerId) const
{
    auto it = mActions.find(button.id);
    if (it == mActions.end()) return InputPassthrough::Passthrough;

    const Amethyst::InputAction& action = *it->second.get();
    return action._onButtonStateChange(button.state, focus, client.asInstance());
}

void Amethyst::InputManager::createKeyboardAndMouseBinding(KeyboardInputMapping* keyboard, MouseInputMapping* mouse, const std::string* buttonName, const std::string* keyName, FocusImpact impact) {
    Keymapping& mapping = *alvinqid::MiniAPI::getInstance().getRMPL()->getKeymappingByAction(*keyName);

	for(int key : mapping.mKeys) {
		//key = mLayout->getAdjustedKey(key);
		if (!mapping.isAssigned()) continue;

        if (mapping.isAltKey()) {
            mouse->buttonBindings.emplace_back(buttonName, key);
		}
        else {
            keyboard->keyBindings.emplace_back(buttonName, key, impact);
        }
	}
}

void Amethyst::InputManager::_registerKeyboardInputs(VanillaClientInputMappingFactory* inputs, KeyboardInputMapping* keyboard, MouseInputMapping* mouse, Amethyst::KeybindContext context)
{
    for (auto& input : mCustomInputs) {
        if ((input.mContexts & context) == Amethyst::KeybindContext::None) {
            continue;
        }

        std::string keyName = "key." + input.mActionName;
        std::string buttonName = "button." + input.mActionName;
        createKeyboardAndMouseBinding(keyboard, mouse, &buttonName, &keyName);
    }
}