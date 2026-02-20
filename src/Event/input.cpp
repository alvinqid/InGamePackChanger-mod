#include "Event/input.hpp"

void EventInput(MiniAPI::InputManager& inputManager) {
    MiniAPI::InputAction& zoomKeybind = inputManager.RegisterNewInput("zoom", {'C'}, true, MiniAPI::KeybindContext::Gameplay);
    zoomKeybind.addButtonDownHandler([](FocusImpact focus, ClientInstance& client) {
        //enabledStart = std::chrono::system_clock::now();
        setEnabled(true);
        return MiniAPI::InputPassthrough::Consume;
    });

    zoomKeybind.addButtonUpHandler([](FocusImpact focus, ClientInstance& client) {
        //enabledStart = std::chrono::system_clock::now();
        setEnabled(false);
        return MiniAPI::InputPassthrough::Consume;
    });
}