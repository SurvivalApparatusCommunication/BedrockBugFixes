#include "BedrockBugFixes.h"

#include <ll/api/Config.h>
#include <ll/api/mod/NativeMod.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/utils/ErrorUtils.h>

namespace bbf {

BedrockBugFixes& BedrockBugFixes::getInstance() {
    static BedrockBugFixes instance;
    return instance;
}

std::filesystem::path BedrockBugFixes::getConfigPath() const {
    return getSelf().getConfigDir() / u8"config.json";
}

bool BedrockBugFixes::loadConfig() {
    bool res{};
    mConfig.emplace();
    try {
        res = ll::config::loadConfig(*mConfig, getConfigPath());
    } catch (...) {
        ll::error_utils::printCurrentException(getLogger());
        res = false;
    }
    if (!res) {
        res = ll::config::saveConfig(*mConfig, getConfigPath());
    }
    return res;
}

bool BedrockBugFixes::saveConfig() {
    return ll::config::saveConfig(*mConfig, getConfigPath());
}

bool BedrockBugFixes::load() {
    if (!loadConfig()) {
        return false;
    }
    return true;
}

bool BedrockBugFixes::enable() {
    if (!mConfig) {
        loadConfig();
    }
    registerConfigCommand();

    return true;
}

bool BedrockBugFixes::disable() {
    saveConfig();
    mConfig.reset();
    return true;
}

bool BedrockBugFixes::unload() { return true; }

} // namespace bbf

LL_REGISTER_MOD(bbf::BedrockBugFixes, bbf::BedrockBugFixes::getInstance());
