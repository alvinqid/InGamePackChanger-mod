#include "mod/api/InputManager.hpp"

#include "mc/client/options/Options.h"
#include "mc/client/options/IOptions.h"
#include "mc/client/renderer/game/LevelRendererPlayer.h"

#include <memory>

namespace alvinqid {

class Zoom {

public:
    static Zoom& getInstance();

    Zoom() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    void setRemappingLayout(RemappingLayout* ptr);
    void setClientInstance(ClientInstance* ptr);
    void setOptions(Options* ptr);

    Options* getOptions() { return mOptions; }
    ClientInstance* getClientInstance() { return mClientInstance; }
    RemappingLayout* getRemappingLayout() { return mRemapLayout; }
private:
    ll::mod::NativeMod& mSelf;
    Options* mOptions = nullptr;
    ClientInstance* mClientInstance = nullptr;
    RemappingLayout* mRemapLayout = nullptr;
public:
    std::unique_ptr<InputManager> mInputManager;
};

} // namespace alvinqid