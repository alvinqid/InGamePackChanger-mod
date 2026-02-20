#include "input/InputAction.hpp"
#include "mc/deps/input/InputHandler.h"

int32_t StringToNameId(const std::string& str)
{
    int32_t hash = -2128831035;

    for (char c : str)
        hash = 16777619 * (hash ^ c);

    return hash;
}

InputAction::InputAction(const std::string& actionName) 
	: mNameHash(StringToNameId("button." + actionName)), mButtonDownHandlers(), mButtonUpHandlers() {}

void InputAction::addButtonDownHandler(std::function<InputPassthrough(FocusImpact, ClientInstance&)> handler)
{
    mButtonDownHandlers.push_back(handler);
}

void InputAction::addButtonUpHandler(std::function<InputPassthrough(FocusImpact, ClientInstance&)> handler)
{
	mButtonUpHandlers.push_back(handler);  
}

InputPassthrough InputAction::_onButtonStateChange(ButtonState state, FocusImpact focus, ClientInstance& client) const
{
    bool shouldPassToVanilla = true;

	if (state == ButtonState::Down) {
		for (const auto& handler : mButtonDownHandlers) {
            InputPassthrough response = handler(focus, client);

            if (response == InputPassthrough::Consume) return response;
            if (response == InputPassthrough::ModOnly) shouldPassToVanilla = false;
		}
	}
	else if (state == ButtonState::Up) {
		for (const auto& handler : mButtonUpHandlers) {
			InputPassthrough response = handler(focus, client);
            if (response == InputPassthrough::Consume) return response;
            if (response == InputPassthrough::ModOnly) shouldPassToVanilla = false;
		}
	}

	return shouldPassToVanilla ? InputPassthrough::Passthrough : InputPassthrough::ModOnly;
}