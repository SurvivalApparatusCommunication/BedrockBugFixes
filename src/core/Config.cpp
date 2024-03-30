#include "Config.h"
#include "BedrockBugFixes.h"

#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/command/runtime/RuntimeOverload.h>
#include <ll/api/reflection/Reflection.h>
#include <ll/api/reflection/Visit.h>

namespace bbf {

enum class bugfix_type {};

void registerConfigCommand() {
    auto& setting = BedrockBugFixes::getInstance().getConfig().commands.config_command;
    if (!setting.enabled) {
        return;
    }
    auto& reg = ll::command::CommandRegistrar::getInstance();

    std::vector<std::pair<std::string, uint64>> bugfixEnum;
    size_t                                      iter{0};
    ll::reflection::forEachMember(
        BedrockBugFixes::getInstance().getConfig().bugfix,
        [&](std::string_view name, auto& val) { bugfixEnum.emplace_back(name, iter++); }
    );
    reg.tryRegisterEnum(ll::command::enum_name_v<bugfix_type>, std::move(bugfixEnum), Bedrock::type_id<CommandRegistry, bugfix_type>(), &CommandRegistry::parse<bugfix_type>);

    auto& cmd = reg.getOrCreateCommand(
        "bbfconfig",
        "config bedrock bugfixes",
        setting.permission
    );

    struct BugfixParam {
        bugfix_type type;
        bool        value;
    };

    cmd.overload<BugfixParam>()
        .text("bugfix")
        .required("type")
        .required("value")
        .execute<[](CommandOrigin const& orgin,
                    CommandOutput&       output,
                    BugfixParam const&   param) {
            ll::meta::visitIndex<boost::pfr::detail::fields_count<Config::BugFixes>()>(
                [&]<size_t I>() {
                    boost::pfr::get<I>(BedrockBugFixes::getInstance().getConfig().bugfix
                    ) = param.value;
                    output.success(
                        "set {} to {}",
                        boost::pfr::get_name<I, Config::BugFixes>(),
                        param.value
                    );
                },
                (size_t)param.type
            );
            BedrockBugFixes::getInstance().saveConfig();
        }>();
}
} // namespace bbf
