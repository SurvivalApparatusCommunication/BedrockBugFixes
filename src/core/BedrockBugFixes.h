#pragma once

#include "Config.h"

#include <ll/api/plugin/NativePlugin.h>

namespace bbf {

class BedrockBugFixes {

public:
    static BedrockBugFixes& getInstance();

    BedrockBugFixes(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    [[nodiscard]] ll::Logger& getLogger() const { return getSelf().getLogger(); }

    [[nodiscard]] Config& getConfig() { return *mConfig; }

    [[nodiscard]] std::filesystem::path getConfigPath() const;

    bool loadConfig();

    bool saveConfig();

    bool load();

    bool enable();

    bool disable();

    bool unload();

private:
    ll::plugin::NativePlugin& mSelf;
    std::optional<Config>     mConfig;
};

} // namespace bbf
