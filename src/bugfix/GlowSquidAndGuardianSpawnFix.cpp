#include "Summary.h"
#include "core/BedrockBugFixes.h"

#include <ll/api/io/FileUtils.h>
#include <ll/api/memory/Hook.h>

#include <mc/resources/IRepositoryFactory.h>
#include <mc/resources/ResourcePackRepository.h>
#include <mc/world/actor/ActorDefinitionIdentifier.h>
#include <mc/world/actor/ActorSpawnRuleGroup.h>
#include <mc/world/actor/SpawnGroupData.h>
#include <mc/world/actor/registry/SpawnGroupRegistry.h>
#include <mc/world/level/BedrockSpawner.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/biome/Biome.h>
#include <mc/world/level/biome/MobSpawnRules.h>
#include <mc/world/level/biome/MobSpawnerData.h>
#include <mc/world/level/biome/registry/BiomeRegistry.h>
#include <mc/world/level/levelgen/v2/SpawnerData.h>

namespace bbf {
static inline ll::SmallDenseSet<HashedString> names{
    {"minecraft:glow_squid", "minecraft:guardian"}
};


LL_TYPE_INSTANCE_HOOK(
    GlowSquidAndGuardianSpawnFixHook,
    ll::memory::HookPriority::Normal,
    BedrockSpawner,
    &BedrockSpawner::$initializeServerSide,
    void,
    ::ResourcePackManager&      rpm,
    ::IWorldRegistriesProvider& registries
) {
    origin(rpm, registries);
    auto& biomeRegistry = registries.getBiomeRegistry();
    biomeRegistry.forEachNonConstBiome([&](::Biome& biome) {
        if (biomeRegistry.biomeHasTag(biome, "overworld")) {
            return;
        }
        erase_if(*biome.mMobs, [](std::shared_ptr<MobSpawnerData> const& mob) {
            return names.contains(
                *mob->mIdentifier->mCanonicalName
            );
        });
        for (auto& map : *biome.mMobsMap) {
            map.erase_if([](auto& dataPair) {
                return names.contains(*dataPair.first.entity->mCanonicalName);
            });
        }
    });
}

struct GlowSquidAndGuardianSpawnFix::Impl {
    ll::memory::HookRegistrar<GlowSquidAndGuardianSpawnFixHook> r;
};

void GlowSquidAndGuardianSpawnFix::call(bool enable) {
    if (enable) {
        if (!impl) impl = std::make_unique<Impl>();
    } else {
        impl.reset();
    }
}

GlowSquidAndGuardianSpawnFix::GlowSquidAndGuardianSpawnFix()  = default;
GlowSquidAndGuardianSpawnFix::~GlowSquidAndGuardianSpawnFix() = default;
} // namespace bbf
