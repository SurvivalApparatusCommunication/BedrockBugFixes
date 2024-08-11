#include "Summary.h"

#include <ll/api/memory/Hook.h>

#include <mc/world/AutomaticID.h>
#include <mc/world/actor/ai/goal/FollowOwnerGoal.h>
#include <mc/world/actor/player/Player.h>

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
