local function copy_all(ext)
    local files = os.files(path.join(os.scriptdir(), ext))
    for _,v in ipairs(files) do
        local src = (path.join(path.directory(path.directory(path.directory(path.directory(v)))), "src/", path.filename(v)))
        if os.exists(src) then
            os.cp(src, v)
        else
            utils.warning("File " .. src .. " not exists.")
        end
    end
end
copy_all("*.h")
copy_all("*.obj")