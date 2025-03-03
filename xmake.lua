add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

if is_config("target_type", "server") then
    add_requires("levilamina 1.0.1", {configs = {target_type = "server"}})
else
    add_requires("levilamina 1.0.1", {configs = {target_type = "client"}})
end

add_requires("levibuildscript")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("target_type")
    set_default("server")
    set_showmenu(true)
    set_values("server", "client")
option_end()

target("BedrockBugFixes")
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
    add_cxflags( "/EHa", "/utf-8", "/W4", "/w44265", "/w44289", "/w44296", "/w45263", "/w44738", "/w45204")
    add_defines("NOMINMAX", "UNICODE")
    add_packages("levilamina")
    set_exceptions("none")
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_includedirs("src")
    -- if is_config("target_type", "server") then
    --     add_includedirs("src-server")
    --     add_files("src-server/**.cpp")
    -- else
    --     add_includedirs("src-client")
    --     add_files("src-client/**.cpp")
    -- end