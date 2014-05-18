

--[[
	{"setLuaCommand", setLuaCommandEx},
	{"showDialog", showDialog},
	{"getCurText", getCurText},
	{"getSelText", getSelText},
	{"getCurLineText", getCurLineText},
	{"getLineText", getLineText},
	{"setCurText", setCurText},
	{"appendCurText", appendCurText},
	{"setNewText", setNewText},
	{"gotoLine", gotoLine},
	{"getLine", getLine},
	{"setSel", setSel},
	{"setStyleColor", setStyleColor},
	{"setStyle", setStyle},
	{"getFileName", getFileName},
	{"openNewFile", openNewFile},

]]--

notepad.setWordStyle = function (work, style)
	
	local text = notepad.getCurText()
	local len = string.len(work)

	local init = -1
	while(init ~= nil) do
		init = string.find(text, work, init+1)
		if init ~= nil then
			notepad.setStyle(init-1, init+len-1, style)
		end
	end
end
