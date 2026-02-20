#include "mod/MiniAPI.hpp"
#include "Event/input.hpp"

namespace alvinqid {

MiniAPI& MiniAPI::getInstance() {
    static MiniAPI instance;
    return instance;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    LevelRendererPlayerFov,
    HookPriority::Normal,
    LevelRendererPlayer,
    &LevelRendererPlayer::getFov,
    float,
    float a, bool b
) {
    return LevelRendererPlayer_getFov(origin(a,b), b);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    BaseOptionsSensi,
    HookPriority::Normal,
    BaseOptions,
    &BaseOptions::$ctor,
    float,
    InputMode inputMode
) {
    return BaseOptions_getSensitivity(origin(inputMode), inputMode);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ClientInstanceCtor,
    HookPriority::Normal,
    ClientInstance,
    &ClientInstance::$ctor,
    float,
    ClientInstanceArguments&& args
) {
    void* self = origin(args);
    
    ClientInstance* ci = reinterpret_cast<ClientInstance*>(self);
    ClientInstance& clientInstance = *ci;
    mInputManager = std::make_unique<InputManager>(ci.getOptionsPtr().get());
    
    return self;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    VanillaClientInputMappingFactorycIMT,
    HookPriority::Normal,
    VanillaClientInputMappingFactory,
    &VanillaClientInputMappingFactory::$createInputMappingTemplates,
    void,
    IOptions* opts
) {
    void* self = origin(opts);
    
    ClientInstance* ci = reinterpret_cast<ClientInstance*>(self);
    ClientInstance& clientInstance = *ci;
    mInputManager = std::make_unique<InputManager>(ci.getOptionsPtr().get());
    EventInput(mInputManager);
    
    return self;
}

bool MiniAPI::load() {
    getSelf().getLogger().debug("Loading...");
    return true;
}

bool MiniAPI::enable() {
    getSelf().getLogger().debug("Enabling...");
    return true;
}

bool MiniAPI::disable() {
    getSelf().getLogger().debug("Disabling...");
    return true;
}

} // namespace alvinqid

LL_REGISTER_MOD(alvinqid::MiniAPI, alvinqid::MiniAPI::getInstance());