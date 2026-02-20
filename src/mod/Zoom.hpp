#include "mc/client/options/BaseOptions.h"
#include "mc/client/renderer/game/LevelRendererPlayer.h"
#include "mc/deps/input/Keyboard.h"
#include "ll/api/mod/NativeMod.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/memory/Hook.h"
#include "mod/Property.hpp"
#include "mc/client/input/VanillaClientInputMappingFactory.h"
#include "mc/client/game/ClientInstance.h"
#include "mc/deps/input/InputHandler.h"

#include "input/InputManager.hpp"

namespace alvinqid {

    class Zoom {
    
    public:
        static Zoom& getInstance();
        Property& getProperty();
        InputManager& getInputManager() const { return mInputManager };
    
        Zoom() : mSelf(*ll::mod::NativeMod::current()) {};
    
        [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; };
    
        bool load();
        bool enable();
        bool disable();
        
    private:
        ll::mod::NativeMod& mSelf;
        InputManager mInputManager;
    };
} // namespace alvinqid