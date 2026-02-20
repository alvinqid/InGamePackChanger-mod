#include "mod/MiniAPI.hpp"

namespace alvinqid {

MiniAPIMOD& MiniAPIMOD::getInstance() {
    static MiniAPIMOD instance;
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
    ClientInstanceCtor,
    HookPriority::Normal,
    ClientInstance,
    &ClientInstance::$ctor,
    void*,
    ClientInstanceArguments&& args
) {
    void* self = origin(args);

    ClientInstance* ci = reinterpret_cast<ClientInstance*>(self);

    MiniAPIMOD::getInstance().setInputManager(
        std::make_unique<MiniAPI::InputManager>(
            ci->getOptionsPtr().get()
        )
    );

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

    auto* inputMgr = MiniAPIMOD::getInstance().getInputManager();
    if (inputMgr) {
        EventInput(*inputMgr);
    }
}

bool MiniAPIMOD::load() {
    getSelf().getLogger().debug("Loading...");
    return true;
}

bool MiniAPIMOD::enable() {
    getSelf().getLogger().debug("Enabling...");
    return true;
}

bool MiniAPIMOD::disable() {
    getSelf().getLogger().debug("Disabling...");
    return true;
}

} // namespace alvinqid

LL_REGISTER_MOD(alvinqid::MiniAPIMOD, alvinqid::MiniAPIMOD::getInstance());