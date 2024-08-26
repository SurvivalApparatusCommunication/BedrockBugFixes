#pragma once

#include "bugfix/Summary.h"

#include <ll/api/reflection/Dispatcher.h>
#include <mc/server/commands/CommandPermissionLevel.h>

namespace bbf {
struct Config {
    int version = 1;

    struct BugFixes {
        ll::reflection::Dispatcher<bool, AllayTeleportBugFix> allay_teleport_bug_fix =
            true;
        ll::reflection::Dispatcher<bool, BeeBugsFix>      bee_bugs_fix       = true;
        ll::reflection::Dispatcher<bool, ChunkSaveBugFix> chunk_save_bug_fix = true;
        ll::reflection::Dispatcher<bool, HopperBugsFix>   hopper_bugs_fix    = true;
    } bugfix{};

    struct CmdSetting {
        bool                   enabled    = true;
        CommandPermissionLevel permission = CommandPermissionLevel::GameDirectors;
    };

    struct {
        CmdSetting config_command{};
    } commands{};
};
void registerConfigCommand();

} // namespace bbf