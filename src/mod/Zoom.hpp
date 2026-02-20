#include "mc/client/options/BaseOptions.h"
#include "mc/client/renderer/game/LevelRendererPlayer.h"
#include "mc/deps/input/Keyboard.h"
#include "ll/api/mod/NativeMod.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/memory/Hook.h"

namespace alvinqid {

    class Zoom {
    
    public:
        static Zoom& getInstance();
    
        Zoom() : mSelf(*ll::mod::NativeMod::current()) {};
    
        [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; };
    
        bool load();
        bool enable();
        bool disable();
        
    private:
        ll::mod::NativeMod& mSelf;
    };
} // namespace alvinqid