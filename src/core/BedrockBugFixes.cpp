#include "BedrockBugFixes.h"

#include <ll/api/Config.h>
#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/plugin/RegisterHelper.h>
#include <ll/api/utils/ErrorUtils.h>

namespace bbf {

static std::unique_ptr<BedrockBugFixes> instance;

BedrockBugFixes& BedrockBugFixes::getInstance() { return *instance; }

bool BedrockBugFixes::loadConfig() {
    bool res{};
    mConfig.emplace();
    try {
        res = ll::config::loadConfig(*mConfig, getSelf().getConfigDir());
    } catch (...) {
        ll::error_utils::printCurrentException(getLogger());
        res = false;
    }
    if (!res) {
        res = ll::config::saveConfig(*mConfig, getSelf().getConfigDir());
    }
    return res;
}

bool BedrockBugFixes::saveConfig() {
    return ll::config::saveConfig(*mConfig, getSelf().getConfigDir());
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
    if (getConfig().commands.config_command) {
        registerConfigCommand();
    }
    return true;
}

bool BedrockBugFixes::disable() {
    saveConfig();
    mConfig.reset();
    return true;
}

bool BedrockBugFixes::unload() { return true; }

} // namespace bbf

LL_REGISTER_PLUGIN(bbf::BedrockBugFixes, bbf::instance);
