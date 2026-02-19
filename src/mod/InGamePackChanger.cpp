#include "mod/InGamePackChanger.hpp"

static bool enabled = false;

namespace alvinqid {

InGamePackChanger& InGamePackChanger::getInstance() {
    static InGamePackChanger instance;
    return instance;
}

//SettingsScreenController
LL_AUTO_TYPE_INSTANCE_HOOK(
    SettingsScreenControllerCtor1,
    HookPriority::Normal,
    SettingsScreenController,
    &SettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    bool createWorld,
    SettingsTabIndex tabIndex,
    bool maintainOldFocus
) {
    void* res = origin(model, createWorld, tabIndex, maintainOldFocus);
    SettingsScreenController* ssc = reinterpret_cast<SettingsScreenController*>(res);
    if(enabled) {
        ssc->mReloadTexturePacksOnExit = true;
        ssc->mIsGlobalSettingsScreen = true;
    }
    
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    SettingsScreenControllerCtor2,
    HookPriority::Normal,
    SettingsScreenController,
    &SettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    Realms::World const& world,
    Realms::InviteLink const& link,
    SettingsTabIndex tabIndex,
    bool maintainOldFocus
) {
    void* res = origin(model, world, link, tabIndex, maintainOldFocus);
    SettingsScreenController* ssc = reinterpret_cast<SettingsScreenController*>(res);
    if(enabled) {
        ssc->mReloadTexturePacksOnExit = true;
        ssc->mIsGlobalSettingsScreen = true;
    }
    
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    SettingsScreenControllerCtor3,
    HookPriority::Normal,
    SettingsScreenController,
    &SettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    Realms::World const& world
    SettingsTabIndex tabIndex,
    bool maintainOldFocus,
    std::string packId
) {
    void* res = origin(model, world, tabIndex, maintainOldFocus, packId);
    SettingsScreenController* ssc = reinterpret_cast<SettingsScreenController*>(res);
    if(enabled) {
        ssc->mReloadTexturePacksOnExit = true;
        ssc->mIsGlobalSettingsScreen = true;
    }
    
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    SettingsScreenControllerCtor4,
    HookPriority::Normal,
    SettingsScreenController,
    &SettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    LevelSummary const& levelSummary,
    SettingsTabIndex tabIndex,
    bool navToMenuOnExit,
    bool maintainOldFocus,
    std::string const& initialPackId
) {
    void* res = origin(model, levelSummary, tabIndex, navToMenuOnExit, maintainOldFocus, initialPackId);
    SettingsScreenController* ssc = reinterpret_cast<SettingsScreenController*>(res);
    if(enabled) {
        ssc->mReloadTexturePacksOnExit = true;
        ssc->mIsGlobalSettingsScreen = true;
    }
    
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    SettingsScreenControllerCtor5,
    HookPriority::Normal,
    SettingsScreenController,
    &SettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    LevelSummary const& levelSummary,
    bool createFromTemplate,
    std::string packId,
    std::function<void()> onOpenCallback,
    SettingsTabIndex tabIndex,
    bool maintainOldFocus
) {
    void* res = origin(model, levelSummary, createFromTemplate, packId, onOpenCallback, tabIndex, maintainOldFocus);
    SettingsScreenController* ssc = reinterpret_cast<SettingsScreenController*>(res);
    if(enabled) {
        ssc->mReloadTexturePacksOnExit = true;
        ssc->mIsGlobalSettingsScreen = true;
    }
    
    return res;
}

//GeneralSettingsScreenController
LL_AUTO_TYPE_INSTANCE_HOOK(
    GeneralSettingsScreenControllerCtor,
    HookPriority::Normal,
    GeneralSettingsScreenController,
    &GeneralSettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    bool isGlobalSettings,
    bool reloadTexturePacksOnExit,
    std::string const& initialPackId
) {
    if(enabled) return origin(model, true, true, initialPackId);
    
    return origin(model, isGlobalSettings, reloadTexturePacksOnExit, initialPackId);
}

bool InGamePackChanger::load() {
    getSelf().getLogger().debug("Loading...");
    return true;
}

bool InGamePackChanger::enable() {
    enabled = true;
    getSelf().getLogger().debug("Enabling...");
    return true;
}

bool InGamePackChanger::disable() {
    enabled = false;
    getSelf().getLogger().debug("Disabling...");
    return true;
}

} // namespace alvinqid

LL_REGISTER_MOD(alvinqid::InGamePackChanger, alvinqid::InGamePackChanger::getInstance());