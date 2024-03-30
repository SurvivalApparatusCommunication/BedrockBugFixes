#include "Summary.h"

#include <ll/api/memory/Hook.h>

#include <mc/math/Vec3.h>
#include <mc/world/AutomaticID.h>
#include <mc/world/actor/ActorDefinitionTrigger.h>
#include <mc/world/actor/ActorFilterGroup.h>
#include <mc/world/actor/ai/goal/MoveToBlockGoal.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/item/ItemDescriptor.h>

namespace bbf {

LL_TYPE_INSTANCE_HOOK(
    MoveToBlockGoalHook,
    ll::memory::HookPriority::Normal,
    MoveToBlockGoal,
    //&MoveToBlockGoal::onPlayerDimensionChanged,
    "??0MoveToBlockGoal@@QEAA@AEAVMob@@MHHHHMVVec3@@MW4TargetSelectionMethod@@V?$vector@"
    "VActorDefinitionTrigger@@V?$allocator@VActorDefinitionTrigger@@@std@@@std@@3V?$"
    "vector@VItemDescriptor@@V?$allocator@VItemDescriptor@@@std@@@5@AEBVActorFilterGroup@"
    "@@Z",
    MoveToBlockGoal*,
    Mob&                                mob,
    float                               speedModifier,
    int                                 searchRange,
    int                                 searchHeight,
    int                                 tickInterval,
    int                                 stayDurationTicks,
    float                               goalRadius,
    Vec3                                targetPositionOffset,
    float                               chanceToStart,
    ::TargetSelectionMethod             targetSelectionMethod,
    std::vector<ActorDefinitionTrigger> onReachTriggers,
    std::vector<ActorDefinitionTrigger> onStayCompletedTriggers,
    std::vector<ItemDescriptor>         targetBlocks,
    ActorFilterGroup const&             filterGroup
) {
    if (mob.isType(ActorType::Bee)) {
        targetSelectionMethod = TargetSelectionMethod::Nearest;
    }
    return origin(
        mob,
        speedModifier,
        searchRange,
        searchHeight,
        tickInterval,
        stayDurationTicks,
        goalRadius,
        targetPositionOffset,
        chanceToStart,
        targetSelectionMethod,
        std::move(onReachTriggers),
        std::move(onStayCompletedTriggers),
        std::move(targetBlocks),
        filterGroup
    );
}

struct BeeBugsFix::Impl {
    ll::memory::HookRegistrar<MoveToBlockGoalHook> r;
};

void BeeBugsFix::call(bool enable) {
    if (enable) {
        if (!impl) impl = std::make_unique<Impl>();
    } else {
        impl.reset();
    }
}

BeeBugsFix::BeeBugsFix()  = default;
BeeBugsFix::~BeeBugsFix() = default;

} // namespace bbf
