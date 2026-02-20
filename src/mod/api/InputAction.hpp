#pragma once

#include <string>
#include <vector>
#include <functional>
#include <type_traits>

#include "mc/deps/input/InputHandler.h"
#include "mc/client/game/IClientInstance.h"
#include "mc/client/game/ClientInstance.h"
#include "mc/deps/input/enums/FocusImpact.h"

class ClientInstance;

int32_t StringToNameId(const std::string& str);

enum class KeybindContext : uint32_t {
    None     = 0,
    Gameplay = 1 << 0,
    Screen   = 1 << 1,
};

inline KeybindContext operator|(KeybindContext lhs, KeybindContext rhs)
{
    using T = std::underlying_type_t<KeybindContext>;
    return static_cast<KeybindContext>(
        static_cast<T>(lhs) | static_cast<T>(rhs)
    );
}

inline KeybindContext operator&(KeybindContext lhs, KeybindContext rhs)
{
    using T = std::underlying_type_t<KeybindContext>;
    return static_cast<KeybindContext>(
        static_cast<T>(lhs) & static_cast<T>(rhs)
    );
}

class InputActionOptions {
public:
    std::string     mActionName;
    std::vector<int> mDefaultKeys;
    bool            mAllowRemapping;
    KeybindContext  mContexts;

    InputActionOptions(
        const std::string& actionName,
        std::vector<int> defaultKeys,
        bool allowRemapping = true,
        KeybindContext contexts = KeybindContext::Gameplay
    )
        : mActionName(actionName),
          mDefaultKeys(std::move(defaultKeys)),
          mAllowRemapping(allowRemapping),
          mContexts(contexts)
    {}
};

enum class InputPassthrough {
    Passthrough,
    Consume,
    ModOnly,
};

class InputAction {
private:
    uint32_t mNameHash{};

    using Handler = std::function<InputPassthrough(FocusImpact, ClientInstance&)>;

    std::vector<Handler> mButtonDownHandlers;
    std::vector<Handler> mButtonUpHandlers;

public:
    explicit InputAction(const std::string& actionName);

    uint32_t getNameHash() const { return mNameHash; }

    void addButtonDownHandler(Handler handler);
    void addButtonUpHandler(Handler handler);

private:
    InputPassthrough _onButtonStateChange(
        ButtonState state,
        FocusImpact focus,
        ClientInstance& client
    ) const;

    friend class InputManager;
};