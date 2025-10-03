#include "Summary.h"

#include <ll/api/memory/Hook.h>

#include <mc/deps/core/string/HashedString.h>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/VanillaBlockTypeIds.h>

namespace bbf {

LL_TYPE_INSTANCE_HOOK(
    BlockRemoveHook,
    ll::memory::HookPriority::Normal,
    Block,
    &Block::_removeFromTickingQueues,
    void,
    BlockSource&    region,
    BlockPos const& pos
) {
    origin(region, pos);
    if (getBlockType().mNameInfo->mFullName.get() == VanillaBlockTypeIds::SnifferEgg()) {
        region._removeFromTickingQueue(pos, *this, TickingQueueType::Random);
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
