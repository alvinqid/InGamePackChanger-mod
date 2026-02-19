#pragma once

#include "ll/api/mod/NativeMod.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"

#include "mc/client/gui/screens/controllers/GeneralSettingsScreenController.h"
#include "mc/client/gui/screens/controllers/MainMenuScreenController.h"
#include "mc/client/gui/screens/controllers/SettingsScreenController.h"

#include <memory>
#include <string>

namespace alvinqid {

class InGamePackChanger {

public:
    static InGamePackChanger& getInstance();

    InGamePackChanger() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    // bool unload();

private:
    ll::mod::NativeMod& mSelf;
    bool enabled = false;
};

} // namespace alvinqid