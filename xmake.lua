-- add_rules("mode.debug", "mode.release")

add_rules("mode.release")

set_languages("c++17")

set_defaultmode("release")
set_optimize("fastest")

add_defines("NDEBUG")

add_rules("plugin.compile_commands.autoupdate", { outputdir = "build64_release" })
add_repositories("local-repo https://github.com/plutolove/xmake-repo.git")

add_requires("fmt 10.0.0")
add_requires("spdlog v1.11.0")
add_requires("boost 1.85.0")
add_requires("gtest v1.13.0")
add_requires("libpg_query duckdb_parser-1.1.3")
add_requires("tabulate v1.5")

target("pluto")
    set_kind("static")
    add_includedirs("./src")
    add_files("src/*/*.cpp")
    add_files("src/*/*/*.cpp")
    add_packages("fmt", "spdlog", "boost", "libpg_query", "tabulate")



target("plutodb")
    set_kind("binary")
    add_includedirs("./src")
    add_files("src/*.cpp")
    add_deps("pluto")
    add_packages("fmt", "spdlog", "boost", "libpg_query", "tabulate")


for _, file in ipairs(os.files("test/test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_files("test/" .. name .. ".cpp")
        add_includedirs("./src")
        add_tests("default")
        add_deps("pluto")
        add_packages("fmt", "spdlog", "boost", "libpg_query", "gtest", "tabulate")
end

