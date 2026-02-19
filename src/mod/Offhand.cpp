#include "mod/Offhand.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"
#include "mc/world/item/VanillaItems.h"
#include "mc/world/item/registry/ItemRegistryRef.h"
#include "mc/world/item/registry/ItemRegistry.h"
#include "mc/world/item/Item.h"

class Experiments;
namespace cereal { struct ReflectionCtx; };
class BaseGameVersion;

namespace alvinqid_offhand {

Offhand& Offhand::getInstance() {
    static Offhand instance;
    return instance;
}

LL_AUTO_TYPE_STATIC_HOOK(
    registerItemsHook,
    HookPriority::Normal,
    VanillaItems,
    &VanillaItems::registerItems,
    void,
    cereal::ReflectionCtx& ctx,
    ItemRegistryRef itemRegistryRef,
    BaseGameVersion const& baseGameVersion,
    Experiments const& experiments
) {
    ItemRegistry* reg = itemRegistryRef._lockRegistry.get();
    ItemRegistry itemRegistry = *reg;
    for (auto& pair : itemRegistry.mIdToItemMap)
    {
        pair.second.get()->setAllowOffhand(true);
    }
    
    origin(ctx, itemRegistryRef, baseGameVersion, experiments);
}

bool Offhand::load() {
    getSelf().getLogger().debug("Loading...");
    return true;
}

bool Offhand::enable() {
    getSelf().getLogger().debug("Enabling...");
    return true;
}

bool Offhand::disable() {
    getSelf().getLogger().debug("Disabling...");
    return true;
}

} // namespace alvinqid_offhand

LL_REGISTER_MOD(alvinqid_offhand::Offhand, alvinqid_offhand::Offhand::getInstance());