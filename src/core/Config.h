#pragma once

#include "bugfix/Summary.h"

#include <ll/api/reflection/Dispatcher.h>

namespace bbf {
struct Config {
    int version = 0;

    struct BugFixes {
       ll::reflection::Dispatcher<bool, HopperBugsFix>       hopper_bugs_fix        = true;
       ll::reflection::Dispatcher<bool, BeeBugsFix>          bee_bugs_fix           = true;
       ll::reflection::Dispatcher<bool, AllayTeleportBugFix> allay_teleport_bug_fix = true;
    } bugfix{};
    struct {
        bool config_command = true;
    } commands{};
};
void registerConfigCommand();

} // namespace bbf