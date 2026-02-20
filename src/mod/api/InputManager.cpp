#include "mod/api/InputManager.hpp"
#include "mc/client/input/KeyboardRemappingLayout.h"
#include "mc/deps/input/ButtonEventData.h"

InputManager::InputManager(alvinqid::Zoom* zoom)
    : mMod(zoom)
{
}

InputAction& InputManager::registerNewInput(
    const std::string& actionName,
    std::vector<int> const& defaultKeys,
    bool allowRemapping,
    KeybindContext context)
{
    const uint32_t hash = StringToNameId("button." + actionName);

    if (auto it = mActions.find(hash); it != mActions.end())
        return *it->second;

    auto layout = mMod->getInstance().getOptions()->getCurrentKeyboardRemapping().get();

    Keymapping keymapping("key." + actionName, defaultKeys);
    keymapping.mAllowRemap = allowRemapping;
    layout->mKeymappings.emplace_back(keymapping);
    layout->mDefaultMappings.emplace_back(keymapping);

    mCustomInputs.emplace_back(
        actionName,
        std::move(defaultKeys),
        allowRemapping,
        context
    );

    auto action = std::make_unique<InputAction>(actionName);
    auto [it, _] = mActions.emplace(hash, std::move(action));

    return *it->second;
}

InputAction& InputManager::getVanillaInput(const std::string& actionName)
{
    const uint32_t hash = StringToNameId("button." + actionName);

    if (auto it = mActions.find(hash); it != mActions.end())
        return *it->second;

    auto action = std::make_unique<InputAction>(actionName);
    auto [it, _] = mActions.emplace(hash, std::move(action));

    return *it->second;
}

InputPassthrough InputManager::_handleButtonEvent(
    InputHandler*,
    const ButtonEventData& button,
    FocusImpact focus,
    IClientInstance& client,
    int) const
{
    auto it = mActions.find(button.id);
    if (it == mActions.end())
        return InputPassthrough::Passthrough;

    const InputAction& action = *it->second;
    return action._onButtonStateChange(
        button.state,
        focus
    );
}

void InputManager::createKeyboardAndMouseBinding(
    KeyboardInputMapping* keyboard,
    MouseInputMapping* mouse,
    const std::string* buttonName,
    const std::string* keyName,
    FocusImpact impact)
{
    Keymapping& mapping = *mMod->getInstance().getRemappingLayout()->getKeymappingByAction(*keyName);

    if (!mapping.isAssigned())
        return;

    for (int key : mapping.mKeys)
    {
        if (mapping.isAltKey())
            mouse->buttonBindings.emplace_back(buttonName, key);
        else
            keyboard->keyBindings.emplace_back(buttonName, key, impact);
    }
}

void InputManager::_registerKeyboardInputs(
    VanillaClientInputMappingFactory*,
    KeyboardInputMapping* keyboard,
    MouseInputMapping* mouse,
    KeybindContext context)
{
    for (auto& input : mCustomInputs)
    {
        if ((input.mContexts & context) == KeybindContext::None)
            continue;

        const std::string keyName    = "key." + input.mActionName;
        const std::string buttonName = "button." + input.mActionName;

        createKeyboardAndMouseBinding(
            keyboard,
            mouse,
            &buttonName,
            &keyName
        );
    }
}