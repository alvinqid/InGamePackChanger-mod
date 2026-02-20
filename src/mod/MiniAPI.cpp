#include "mod/MiniAPI.hpp"

#include "features/zoom.hpp"

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
    &BaseOptions::$getSensitivity,
    float,
    InputMode inputMode
) {
    return BaseOptions_getSensitivity(origin(inputMode));
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    RemappingLayoutCtor,
    HookPriority::Normal,
    RemappingLayout,
    &RemappingLayout::$ctor,
    void*
) {
    void* self = origin();

    MiniAPI::getInstance().getRMPL() =
        reinterpret_cast<RemappingLayout*>(self);

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

    MiniAPI::getInstance().getClientInstance() =
        reinterpret_cast<ClientInstance*>(self);

    return self;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    VanillaClientInputMappingFactorycIMT,
    HookPriority::Normal,
    VanillaClientInputMappingFactory,
    &VanillaClientInputMappingFactory::$createInputMappingTemplates,
    void,
    IOptions& opts
) {
    origin(opts);

    auto& api = MiniAPI::getInstance();
    auto clientInstance = api.getClientInstance();
    auto rmpl = api.getRMPL();

    if (clientInstance && rmpl) {
        Amethyst::InputManager inputMgr(
            clientInstance->getOptionsPtr().get()
        );

        EventInput(inputMgr);
    }
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