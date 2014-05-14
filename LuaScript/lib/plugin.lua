require "notepad"

luaPath = string.match(luaFile, "(.+)\\[^\\]*%.%w+$") .. "\\"

dofile(luaPath .. "lib\\util.lua")
dofile(luaPath .. "lib\\notepad.lua")


plugin = {}

local func, err = loadfile(luaFile)

if func ~= nil then
	local status, err = pcall(function () func() end)
	if not status then
		notepad.setNewText("Lua file error: \n" .. err)
	end

else
	notepad.setNewText("Lua file build error: \n" .. err)
end

