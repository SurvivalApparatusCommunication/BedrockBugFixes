#include "Summary.h"

#include <ll/api/memory/Hook.h>

class LevelStorageWriteBatch;

#include <mc/world/level/chunk/LevelChunk.h>
#include <mc/world/level/storage/DBChunkStorage.h>

namespace bbf {

static thread_local bool isInSerializeChunk{false};

LL_TYPE_INSTANCE_HOOK(
    LevelChunkMinYHook,
    ll::memory::HookPriority::Normal,
    LevelChunk,
    &LevelChunk::getMinY,
    short,
) {
    if (isInSerializeChunk) {
        return origin() - 16;
    }
    return origin();
}

LL_TYPE_INSTANCE_HOOK(
    SerializeChunkHook,
    ll::memory::HookPriority::Normal,
    DBChunkStorage,
    &DBChunkStorage::_serializeChunk,
    void,
    LevelChunk&             lc,
    LevelStorageWriteBatch& batch,
    bool                    b
) {
    isInSerializeChunk = true;
    origin(lc, batch, b);
    isInSerializeChunk = false;
}


struct ChunkSaveBugFix::Impl {
    ll::memory::HookRegistrar<LevelChunkMinYHook, SerializeChunkHook> r;
};

void ChunkSaveBugFix::call(bool enable) {
    if (enable) {
        if (!impl) impl = std::make_unique<Impl>();
    } else {
        impl.reset();
    }
}

ChunkSaveBugFix::ChunkSaveBugFix()  = default;
ChunkSaveBugFix::~ChunkSaveBugFix() = default;

} // namespace bbf
