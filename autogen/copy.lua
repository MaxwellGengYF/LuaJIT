local lib = import('lib', {rootdir = path.join(os.projectdir(), "xmake")})
local host = os.host()
local arch = os.arch()
local function exec(sb, cmd)
    sb:add(cmd):add("\r\n")
end
local function find_msvc(sb)
    import("detect.sdks.find_vstudio")
    local tool = find_vstudio()
    local max_version = 0
    local key = nil
    for version, dict in pairs(tool) do
        local ver_num = tonumber(version)
        if ver_num >= max_version then
            max_version = ver_num
            key = version
        end
        break
    end
    if not key then
        target:set_enabled(false)
        utils.error("Can not find Visual Studio.")
        return
    end
    local vcvarsall_bat = tool[key]["vcvarsall_bat"]
    local vcvarsall = tool[key]["vcvarsall"]
    if vcvarsall_bat then
        exec(sb, 'call "' .. vcvarsall_bat .. '" ' .. arch)
    end
end
local sb = lib.StringBuilder()
local tmp_file = path.join(os.scriptdir(), "_tmp.cmd")
find_msvc(sb)
exec(sb, "cd src")
exec(sb, "call msvcbuild.bat")
sb:write_to(tmp_file)
sb:dispose()
os.exec(tmp_file)
os.rm(tmp_file)

local function copy_all(ext)
    local files = os.files(path.join(os.scriptdir(), ext))
    for _,v in ipairs(files) do
        local src = (path.join(os.projectdir(), "src", path.filename(v)))
        if os.exists(src) then
            os.cp(src, v)
            os.rm(src)
        else
            utils.warning("File " .. src .. " not exists.")
        end
    end
end
copy_all(path.join(host, arch, "*.h"))
copy_all(path.join(host, arch, "*.obj"))
copy_all(path.join(host, arch, "*.o"))
copy_all(path.join(host, arch, "*.S"))