#pragma once

#include <memory>

#include "ll/api/mod/NativeMod.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"

#include "mc/client/input/VanillaClientInputMappingFactory.h"
#include "input/InputManager.hpp"
#include "mc/client/renderer/game/LevelRendererPlayer.h"
#include "mc/deps/input/InputMode.h"
#include "mc/client/options/BaseOptions.h"
#include "mc/client/input/RemappingLayout.h"

#include "mc/client/game/ClientInstance.h"

#include "Event/input.hpp"

namespace alvinqid {

class MiniAPI {

public:

    static MiniAPI& getInstance();

    MiniAPI() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    //bool unload();

private:
    ll::mod::NativeMod& mSelf;
    ClientInstance* clientInstance;
    RemappingLayout* rmpl;
};

public:
    void setClientInstance(ClientInstance* ci) { clientInstance = ci; }
    void setRMPL(RemappingLayout* r) { rmpl = r; }

} // namespace alvinqid