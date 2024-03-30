#include "Summary.h"

#include <ll/api/memory/Hook.h>

#include <mc/world/AutomaticID.h>
#include <mc/world/actor/ai/goal/FollowOwnerGoal.h>
#include <mc/world/actor/player/Player.h>

namespace bbf {

LL_TYPE_INSTANCE_HOOK(
    FollowOwnerGoalHook,
    ll::memory::HookPriority::Normal,
    FollowOwnerGoal,
    //&FollowOwnerGoal::onPlayerDimensionChanged,
    "?onPlayerDimensionChanged@FollowOwnerGoal@@UEAAXPEAVPlayer@@V?$AutomaticID@"
    "VDimension@@H@@1@Z",
    void,
    Player*       player,
    DimensionType fromDimension,
    DimensionType toDimension
) {
    auto& mob = *ll::memory::dAccess<Mob*>(this, 7 * 8);
    if (mob.isType(ActorType::Allay)) {
        return;
    }
    origin(player, fromDimension, toDimension);
}

struct AllayTeleportBugFix::Impl {
    ll::memory::HookRegistrar<FollowOwnerGoalHook> r;
};

void AllayTeleportBugFix::call(bool enable) {
    if (enable) {
        if (!impl) impl = std::make_unique<Impl>();
    } else {
        impl.reset();
    }
}

AllayTeleportBugFix::AllayTeleportBugFix()  = default;
AllayTeleportBugFix::~AllayTeleportBugFix() = default;

} // namespace bbf
