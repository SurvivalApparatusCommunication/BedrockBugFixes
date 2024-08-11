#include "Summary.h"

#include <ll/api/memory/Hook.h>

#include <mc/world/level/BlockSource.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/utils/VanillaBlockTypeIds.h>

namespace bbf {

LL_TYPE_INSTANCE_HOOK(
    BlockRemoveHook,
    ll::memory::HookPriority::Normal,
    Block,
    &Block::onRemove,
    void,
    BlockSource&    region,
    BlockPos const& pos
) {
    origin(region, pos);
    if (getTypeName() == VanillaBlockTypeIds::SnifferEgg.getString()) {
        region.removeFromRandomTickingQueue(pos, *this);
    }
}

struct SnifferEggBugFix::Impl {
    ll::memory::HookRegistrar<BlockRemoveHook> r;
};

void SnifferEggBugFix::call(bool enable) {
    if (enable) {
        if (!impl) impl = std::make_unique<Impl>();
    } else {
        impl.reset();
    }
}

SnifferEggBugFix::SnifferEggBugFix()  = default;
SnifferEggBugFix::~SnifferEggBugFix() = default;

} // namespace bbf
