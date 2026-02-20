#include "MiniAPI/input/InputAction.hpp"
#include "mc/deps/input/InputHandler.h"

class ClientInstance;

MiniAPI::InputAction::InputAction(const std::string& actionName) 
	: mNameHash(StringToNameId("button." + actionName)), mButtonDownHandlers(), mButtonUpHandlers() {}

void MiniAPI::InputAction::addButtonDownHandler(std::function<InputPassthrough(FocusImpact, ClientInstance&)> handler)
{
    mButtonDownHandlers.push_back(handler);
}

void MiniAPI::InputAction::addButtonUpHandler(std::function<InputPassthrough(FocusImpact, ClientInstance&)> handler)
{
	mButtonUpHandlers.push_back(handler);  
}

MiniAPI::InputPassthrough MiniAPI::InputAction::_onButtonStateChange(ButtonState state, FocusImpact focus, ClientInstance& client) const
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