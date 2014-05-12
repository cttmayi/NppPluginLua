require "notepad"

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

--[[
plugin.doLuaCommand = function()
	notepad.setNewText(luaFile)
	--dofile(luaFile)
end

plugin.main = function()
	plugin.doLuaCommand()
end

plugin.main()

]]--

