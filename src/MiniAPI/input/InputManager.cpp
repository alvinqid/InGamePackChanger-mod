#include <utility>
#include <algorithm>

#include "MiniAPI/input/InputManager.hpp"
#include "MiniAPI/input/InputAction.hpp"

#include <mc/client/options/IOptions.h>
#include <mc/client/input/KeyboardRemappingLayout.h>
#include <mc/client/input/Keymapping.h>
#include <mc/client/input/VanillaClientInputMappingFactory.h>
#include <mc/deps/input/InputHandler.h>
#include <mc/client/game/IClientInstance.h>

MiniAPI::InputManager::InputManager(Options* opt)
    : mOptions(opt) {}

MiniAPI::InputManager::~InputManager()
{
    if (!mOptions) return;

    auto& layouts = mOptions->mKeyboardRemappings.get();

    for (auto& action : mCustomInputs)
    {
        for (auto& weakLayout : layouts)
        {
            if (auto layout = weakLayout.lock())
            {
                auto& keymappings = layout->mKeymappings;

                auto newEnd = std::remove_if(
                    keymappings.begin(),
                    keymappings.end(),
                    [&](const Keymapping& mapping)
                    {
                        return mapping.mAction == "key." + action.mActionName;
                    });

                keymappings.erase(newEnd, keymappings.end());
            }
        }
    }

    mCustomInputs.clear();
    mActions.clear();
}

MiniAPI::InputAction& MiniAPI::InputManager::RegisterNewInput(
    const std::string& actionName,
    std::vector<int> defaultKeys,
    bool allowRemapping,
    KeybindContext context)
{
    if (context == KeybindContext::None)
        throw std::runtime_error("Invalid keybind context");

    if (!mOptions)
        throw std::runtime_error("Options is nullptr");

    uint32_t hash = StringToNameId("button." + actionName);

    auto it = mActions.find(hash);
    if (it != mActions.end())
        return *(it->second);

    auto& layouts = mOptions->mKeyboardRemappings.get();

    for (auto& weakLayout : layouts)
    {
        if (auto layout = weakLayout.lock())
        {
            Keymapping keymapping(
                "key." + actionName,
                defaultKeys,
                allowRemapping
            );

            layout->mKeymappings.emplace_back(keymapping);
            layout->mDefaultMappings.emplace_back(keymapping);
        }
    }

    mCustomInputs.emplace_back(actionName, std::move(defaultKeys), allowRemapping, context);

    auto action = std::make_unique<MiniAPI::InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

MiniAPI::InputAction& MiniAPI::InputManager::GetVanillaInput(const std::string& actionName)
{
    uint32_t hash = StringToNameId("button." + actionName);

    auto it = mActions.find(hash);
    if (it != mActions.end())
        return *(it->second);

    auto action = std::make_unique<MiniAPI::InputAction>(actionName);
    auto [newIt, _] = mActions.emplace(hash, std::move(action));
    return *(newIt->second);
}

MiniAPI::InputPassthrough MiniAPI::InputManager::_handleButtonEvent(
    InputHandler*,
    const ButtonEventData& button,
    FocusImpact focus,
    IClientInstance& client,
    int) const
{
    auto it = mActions.find(button.id);
    if (it == mActions.end())
        return InputPassthrough::Passthrough;

    const MiniAPI::InputAction& action = *it->second.get();
    return action._onButtonStateChange(button.state, focus, client);
}

void MiniAPI::InputManager::createKeyboardAndMouseBinding(
    VanillaClientInputMappingFactory* inputs,
    KeyboardInputMapping* keyboard,
    MouseInputMapping* mouse,
    const std::string* buttonName,
    const std::string* keyName,
    FocusImpact impact)
{
    if (auto layout = inputs->mKeyboardRemappingLayout.lock())
    {
        Keymapping* mapping = layout->getKeymappingByAction(*keyName);
        if (!mapping) return;

        for (int key : mapping->mKeys)
        {
            if (!mapping->isAssigned())
                continue;

            if (mapping->isAltKey())
                mouse->buttonBindings.emplace_back(buttonName, key);
            else
                keyboard->keyBindings.emplace_back(buttonName, key, impact);
        }
    }
}

void MiniAPI::InputManager::_registerKeyboardInputs(
    VanillaClientInputMappingFactory* inputs,
    KeyboardInputMapping* keyboard,
    MouseInputMapping* mouse,
    MiniAPI::KeybindContext context)
{
    for (auto& input : mCustomInputs)
    {
        if ((input.mContexts & context) == MiniAPI::KeybindContext::None)
            continue;

        std::string keyName = "key." + input.mActionName;
        std::string buttonName = "button." + input.mActionName;

        createKeyboardAndMouseBinding(inputs, keyboard, mouse, &buttonName, &keyName);
    }
}