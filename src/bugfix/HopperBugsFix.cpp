#include "Summary.h"

#include <ll/api/memory/Hook.h>

#include <mc/entity/WeakEntityRef.h>
#include <mc/math/Vec3.h>
#include <mc/world/Container.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/Hopper.h>
#include <mc/world/actor/item/ItemActor.h>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/ChunkPos.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/utils/VanillaBlockTypeIds.h>
#include <mc/world/level/chunk/LevelChunk.h>
#include <mc/world/phys/AABB.h>

namespace bbf {

LL_TYPE_INSTANCE_HOOK(
    HopperTryAddItemHook,
    ll::memory::HookPriority::Normal,
    Hopper,
    &Hopper::_tryAddItemsFromPos,
    bool,
    BlockSource& blockSource,
    Container&   container,
    Vec3 const&  pos
) {
    AABB aabb;

    if (mIsEntity) {
        aabb.min = pos - Vec3{0.5f, 0.0f, 0.5f};
        aabb.max = pos + Vec3{0.5f, 1.0f, 0.5f};
    } else {
        aabb.min    = pos;
        aabb.min.y -= 0.375f;
        aabb.max    = pos + 1;
    }

    ChunkPos minChunk(
        ((int)std::floor(aabb.min.x - 0.125f)) >> 4,
        ((int)std::floor(aabb.min.z - 0.125f)) >> 4
    );
    ChunkPos maxChunk(
        ((int)std::floor(aabb.max.x + 0.125f)) >> 4,
        ((int)std::floor(aabb.max.z + 0.125f)) >> 4
    );

    for (int x = minChunk.x; x <= maxChunk.x; x++)
        for (int z = minChunk.z; z <= maxChunk.z; z++) {
            auto* chunk = blockSource.getChunk({x, z});
            if (!chunk) {
                continue;
            }
            for (auto& weakEntityRef : chunk->getChunkEntities()) {
                auto* actor = weakEntityRef.tryUnwrap<Actor>().as_ptr();
                if (!actor || !actor->isType(ActorType::ItemEntity)
                    || !aabb.intersects(actor->getAABB()) || actor->isRemoved()) {
                    continue;
                }
                auto& item = static_cast<ItemActor*>(actor)->item();
                if (item.isNull()) {
                    continue;
                }
                if (_addItem(blockSource, container, item, -1, item.mCount)) {
                    if (item.mCount != 0) {
                        _addItem(
                            blockSource,
                            container,
                            item,
                            -1,
                            item.mCount
                        ); // need to add twice
                    }
                    if (item.mCount == 0) {
                        actor->remove();
                    }
                    return true;
                }
            }
        }
    return false;
}

LL_TYPE_INSTANCE_HOOK(
    IsContainerBlockHook,
    ll::memory::HookPriority::Normal,
    Block,
    &Block::isContainerBlock,
    bool
) {
    return getName() == VanillaBlockTypeIds::Composter || origin();
}

struct HopperBugsFix::Impl {
    ll::memory::HookRegistrar<HopperTryAddItemHook, IsContainerBlockHook> r;
};

void HopperBugsFix::call(bool enable) {
    if (enable) {
        if (!impl) impl = std::make_unique<Impl>();
    } else {
        impl.reset();
    }
}

HopperBugsFix::HopperBugsFix()  = default;
HopperBugsFix::~HopperBugsFix() = default;

} // namespace bbf
