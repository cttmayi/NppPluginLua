require "notepad"
--dofile("lib/plugin.lua")


string.split = function(s, p)

    local rt= {}
    string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end )
    return rt

end

android = {}

android.filter =  function (str, list) 
	for _, s in ipairs(list) do
		local l, _ = string.find(str, s)
		if (l ~= nil) then
			return true
		end
	end
	return false
end


str = string.gsub(notepad.getCurText(), "\r\n", "\n") 

local list = string.split(str, '\n')

out = ""

for _, s in ipairs(list) do
	if android.filter(s, {"ActivityManager:", "ActivityThread:"}) then
		out = out .. s 
	end
end


notepad.setNewText(out)