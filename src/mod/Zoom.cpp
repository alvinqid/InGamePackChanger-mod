#include "mod/Zoom.hpp"
#include "config.h"
#include "ll/api/Config.h"

static bool enabled = false;

static void setIsEnabled(bool a) {
    enabled = a;
}

static bool isEnabled()
{
/*
    auto& keyStates = Keyboard::_states();
    return keyStates[Keyboard::C] != 0;
*/
    return enabled;
}

namespace alvinqid {
    Property props
    Config config;

    LL_AUTO_TYPE_INSTANCE_HOOK(
        ClientInstanceCtor,
        HookPriority::Normal,
        ClientInstance,
        &ClientInstance::$ctor,
        void*,
        ClientInstanceArguments&& args
    ) {
        void* self = origin(args);
    
        props.client = reinterpret_cast<ClientInstance*>(self);
        
        return self;
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        VanillaClientInputMappingFactoryA,
        HookPriority::Normal,
        VanillaClientInputMappingFactory,
        &VanillaClientInputMappingFactory::$createInputMappingTemplates,
        void,
        IOptions& opts
    ) {
        origin(opts);
        props.keyboardLayout = opts.getCurrentKeyboardRemapping().get();
    
        InputAction& zoomKeybind = Zoom::getInstance().getInputManager().registerNewInput("zoom", {'C'}, true, KeybindContext::Gameplay);
    
        zoomKeybind.addButtonDownHandler([](FocusImpact focus) {
            setIsEnabled(true);
            return InputPassthrough::Consume;
        });
    
        zoomKeybind.addButtonUpHandler([](FocusImpact focus) {
            setIsEnabled(false);
            return InputPassthrough::Consume;
        });
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        VanillaClientInputMappingFactoryB,
        HookPriority::Normal,
        VanillaClientInputMappingFactory,
        &VanillaClientInputMappingFactory::_addFullKeyboardGamePlayControls,
        void,
        KeyboardInputMapping& keyboardMapping,
        MouseInputMapping& mouseMapping,
        bool withInventoryCycle
    ) {
        origin(keyboardMapping,mouseMapping,withInventoryCycle);
        Zoom::getInstance().getInputManager()->_registerKeyboardInputs(&keyboardMapping, &mouseMapping, KeybindContext::Gameplay);
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        VanillaClientInputMappingFactoryC,
        HookPriority::Normal,
        VanillaClientInputMappingFactory,
        &VanillaClientInputMappingFactory::_createScreenKeyboardAndMouseMapping,
        void,
        KeyboardInputMapping& screenKeyboardMapping,
        MouseInputMapping& screenMouseMapping
    ) {
        origin(screenKeyboardMapping,screenMouseMapping);
        Zoom::getInstance().getInputManager()->_registerKeyboardInputs(&screenKeyboardMapping, &screenMouseMapping, KeybindContext::Screen);
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        InputHandlerEvent,
        HookPriority::Normal,
        InputHandler,
        &InputHandler::_handleButtonEvent,
        void,
        ButtonEventData const& button,
        FocusImpact focusImpact,
        IClientInstance& client,
        int controllerId
    ) {
        InputPassthrough passthrough = Zoom::getInstance().getInputManager()->_handleButtonEvent(button, focusImpact, client, controllerId);
    	if (passthrough == InputPassthrough::Passthrough) {
    	    origin(button, focusImpact, client, controllerId);
    	}
    }

    LL_AUTO_TYPE_INSTANCE_HOOK(
        LevelRendererPlayerFov,
        HookPriority::Normal,
        LevelRendererPlayer,
        &LevelRendererPlayer::getFov,
        float,
        float a, bool b
    ) {
        float currentFov = origin(a,b);
        if(currentFov == 70.0f && !b) return currentFov;
        if(isEnabled()) return config.targetFov;
    
        return currentFov;
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        BaseOptionsSensi,
        HookPriority::Normal,
        BaseOptions,
        &BaseOptions::$getSensitivity,
        float,
        InputMode inputMode
    ) {
        float currentSensitivity = origin(inputMode);
        float dampen = (100.0f - config.sensitivityDampen) / 100.0f;
        float targetSensitivity = currentSensitivity * dampen;
        
        if(isEnabled()) return targetSensitivity;
    
        return currentSensitivity;
    }
    
    Property& Zoom::getProperty() {
        return props;
    }
    
    Zoom& Zoom::getInstance() {
        static Zoom instance;
        return instance;
    }
    
    bool Zoom::load() {
        const auto& configFilePath = getSelf().getConfigDir() / "config.json";
        if (!ll::config::loadConfig(config, configFilePath)) {
            ll::config::saveConfig(config, configFilePath);
        }
        getSelf().getLogger().debug("Loading...");
        return true;
    }
    
    bool Zoom::enable() {
        getSelf().getLogger().debug("Enabling...");
        return true;
    }
    
    bool Zoom::disable() {
        getSelf().getLogger().debug("Disabling...");
        return true;
    }

} // namespace alvinqid

LL_REGISTER_MOD(alvinqid::Zoom, alvinqid::Zoom::getInstance());