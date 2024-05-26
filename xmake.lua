set_xmakever('2.9.1')
add_rules('mode.release', 'mode.debug')
-- disable ccache in-case error
set_policy('build.ccache', false)

option("lua_custom_alloc")
set_showmenu(true)
set_default(false)
option_end()

-- disable jit compiler for redhat and centos
local jit = true
local plat = "$(plat)"
local arch = "$(arch)"
if is_plat("msys", "mingw", "cygwin") then
    plat = "windows"
    arch = is_arch("x86_64") and "x64" or "x86"
elseif is_plat("android") then
    plat = "linux"
end
if is_arch("arm64", "arm64-v8a") then
    arch = "arm64"
elseif is_arch("arm.*") then
    arch = "arm"
elseif is_arch("mips64.*") then
    arch = "mips64"
    jit = false
end
if os.isfile("/etc/redhat-release") then
    jit = false
end
local autogendir = path.join("autogen", plat, arch)
includes("xmake/xmake_func.lua")
if not get_config("lua_custom_alloc") then
includes ('src/mimalloc')
end
target("luajit_lib")

    -- make as a static library
    _config_project({
        project_kind = "shared"
    })
    if not get_config("lua_custom_alloc") then
        add_deps('mimalloc')
    else
        add_defines("LUA_USE_CUSTOM_ALLOCATOR")
    end
    add_includedirs("src", {public = true})

    -- add the common source files
    add_files("src/*.c|ljamalg.c|luajit.c")
    if is_plat("windows") then
        add_files(autogendir .. "/lj_vm.obj")
        add_defines("LUA_BUILD_AS_DLL", {public = true})
        add_defines("LUA_CORE")
    elseif is_plat("msys", "cygwin", "mingw") then
        add_files(autogendir .. "/lj_vm.o")
    else
        add_files(autogendir .. "/*.S")
    end
    add_includedirs(autogendir, {public = true})

    add_defines("LUAJIT_USE_SYSMALLOC", {public = true})
    if is_plat("linux") then
        add_defines("MAP_ANONYMOUS")
        add_defines("LJ_PROFILE_SIGPROF", {public = true})
    end

    -- disable jit compiler?
    if not jit then
        add_defines("LUAJIT_DISABLE_JIT", {public = true})
    end

    -- fix call math.sin/log crash for fedora/i386/lj_vm.S with `LDFLAGS = -specs=/usr/lib/rpm/redhat/redhat-hardened-ld` in xmake.spec/%set_build_flags
    if is_plat("linux") and is_arch("i386") then
        add_asflags("-fPIE", {public = true})
        add_ldflags("-fPIE", {public = true})
        add_defines("LJ_NO_UNWIND", {public = true})
    end

    -- enable lua5.2 compat, @see http://luajit.org/extensions.html
    --[[
    add_defines("LUAJIT_ENABLE_LUA52COMPAT")
    if not is_plat("windows") then
        add_cflags("-Wno-error=unused-function")
    end]]
target_end()

target("luajit")
    _config_project({
        project_kind = "binary"
    })
    add_deps("luajit_lib")
    add_files("src/luajit.c")
    add_files("libc/**.cpp")
target_end()
