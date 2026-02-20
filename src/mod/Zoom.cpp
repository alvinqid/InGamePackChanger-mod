#include "mod/Zoom.hpp"

namespace alvinqid {

    Zoom& Zoom::getInstance() {
        static Zoom instance;
        return instance;
    }
    
    void Zoom::setRemappingLayout(RemappingLayout* ptr) {
        mRemapLayout = ptr;
    }
    
    void Zoom::setClientInstance(ClientInstance* ptr) {
        mClientInstance = ptr;
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        LevelRendererPlayerFov,
        HookPriority::Normal,
        LevelRendererPlayer,
        &LevelRendererPlayer::getFov,
        float,
        float a, bool b
    ) {
        return alvinqid::Features::getFov(origin(a,b), b);
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        BaseOptionsSensi,
        HookPriority::Normal,
        BaseOptions,
        &BaseOptions::$getSensitivity,
        float,
        InputMode inputMode
    ) {
        return alvinqid::Features::getSensitivity(origin(inputMode));
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        RemappingLayoutCtor,
        HookPriority::Normal,
        RemappingLayout,
        &RemappingLayout::$ctor,
        void*
    ) {
        void* self = origin();
    
        Zoom::getInstance().setRemappingLayout(reinterpret_cast<RemappingLayout*>(self));
    
        return self;
    }
    
    LL_AUTO_TYPE_INSTANCE_HOOK(
        ClientInstanceCtor,
        HookPriority::Normal,
        ClientInstance,
        &ClientInstance::$ctor,
        void*,
        ClientInstanceArguments&& args
    ) {
        void* self = origin(args);
    
        Zoom::getInstance().setClientInstance(reinterpret_cast<ClientInstance*>(self));
        Zoom::getInstance().setOptions(Zoom::getInstance().getClientInstance()->getOptionsPtr().get());
        
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
    
        alvinqid::Features::RegisterCustomInputs(mInputManager);
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
        mInputManager->_registerKeyboardInputs(&keyboardMapping, &mouseMapping, KeybindContext::Gameplay);
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
        mInputManager->_registerKeyboardInputs(&screenKeyboardMapping, &screenMouseMapping, KeybindContext::Screen);
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
        InputPassthrough passthrough = mInputManager->_handleButtonEvent(button, focusImpact, client, controllerId);
    	if (passthrough == InputPassthrough::Passthrough) {
    	    origin(button, focusImpact, client, controllerId);
    	}
    }
    
    bool Zoom::load() {
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