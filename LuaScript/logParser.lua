


log = {}

log.split = function (log)
	local date = string.sub(log, 1, 5)
	local time = string.sub(log, 6, 18)
	local pid = string.sub(log, 20, 25)
	local tid = string.sub(log, 26, 30)
	local level = string.sub(log, 32, 32)
	local t = string.sub(log, 34)
	if t ~= nil then
		local l = string.find(t, ":")
		if l ~= nil then
			local tag = string.sub(t, 1, l - 1)
			local msg = string.sub(t, l + 2)
			return date,time, pid, tid, level, tag, msg
		end
	end
	return
end

log.getValue = function (str, pa)
	t = {}
	for k, v in string.gmatch(str, "(%w+)=([%g]+)") do
		t[k] = v
	end
	return t
end

--log.logCheck = function (tag, msg, )

-- tag, msg, condition, report
log.reportInfos = {
	{"ActivityManager", "Broadcast sticky", [[ return vlaues["ordered"] == "false" ]], [[ return "Message Error:" .. pid ]]},
	{"WifiController", "DeviceActiveState", [[ return (tonumber(vlaues["what"])) > 10 ]], [[ return "数据:" .. pid ]]},
					}


local datas = {}


local logs

logs = string.gsub(notepad.getCurText(), "\r", "")

local logLines = string.split(logs, "\n")


local r = ""
for k, logLine in pairs(logLines) do
	date, time, pid, tid, level, tag, msg = log.split(logLine)
	if time ~= nil then
		vlaues = log.getValue(msg)

		for tk, reportInfo in pairs(log.reportInfos) do
			if tag == reportInfo[1] then
				if reportInfo[2] ~= nil and string.find(msg, reportInfo[2]) ~= nil then
					if reportInfo[3] ~= nil and loadstring(reportInfo[3])() == true then
						local text = loadstring(reportInfo[4])()
						if text ~= nil then
							r = r .. logLine .. "\n"
							r = r .. text .. "\n\n"
						end
					end
				end
			end
		end
	end
end

notepad.setNewText(r)






