#include "mod/api/InputAction.hpp"

class ClientInstance;

int32_t StringToNameId(const std::string& str)
{
    int32_t hash = -2128831035;

    for (char c : str)
        hash = 16777619 * (hash ^ c);

    return hash;
}

InputAction::InputAction(const std::string& actionName)
    : mNameHash(StringToNameId("button." + actionName))
{
}

void InputAction::addButtonDownHandler(
    std::function<InputPassthrough(FocusImpact)> handler)
{
    mButtonDownHandlers.emplace_back(std::move(handler));
}

void InputAction::addButtonUpHandler(
    std::function<InputPassthrough(FocusImpact)> handler)
{
    mButtonUpHandlers.emplace_back(std::move(handler));
}

InputPassthrough InputAction::_onButtonStateChange(
    ButtonState state,
    FocusImpact focus) const
{
    const auto& handlers =
        (state == ButtonState::Down)
        ? mButtonDownHandlers
        : mButtonUpHandlers;

    bool passToVanilla = true;

    for (const auto& handler : handlers)
    {
        InputPassthrough result = handler(focus);

        if (result == InputPassthrough::Consume)
            return InputPassthrough::Consume;

        if (result == InputPassthrough::ModOnly)
            passToVanilla = false;
    }

    return passToVanilla
        ? InputPassthrough::Passthrough
        : InputPassthrough::ModOnly;
}