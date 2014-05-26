

log = {}

-- tag, msg, condition, post
processInfos = {
	{"ActivityManager", nil, nil, [[ log.setPid(log.pid, "system_process")  ]]},
	{"SurfaceFlinger", nil, nil, [[ log.setPid(log.pid, "surfaceflinger")  ]]},
	{"MediaPlayerService", nil, nil, [[ log.setPid(log.pid, "mediaserver")  ]]},
	{"AudioFlinger", nil, nil, [[ log.setPid(log.pid, "mediaserver")  ]]},
	{"ADB_SERVICES", nil, nil, [[ log.setPid(log.pid, "adbd")  ]]},
	{"DrmMtkPlugIn", nil, nil, [[ log.setPid(log.pid, "drm")  ]]},
	{"Launcher3", nil, nil, [[ log.setPid(log.pid, "com.android.launcher3")  ]]},
	{"KeyguardHostView", nil, nil, [[ log.setPid(log.pid, "com.android.keyguard")  ]]},
	{"WallpaperService", nil, nil, [[ log.setPid(log.pid, "com.android.systemui")  ]]},
}


flowInfos = {
	{"ActivityManager", "wakingUp", nil, [[ log.add("Operator:", "Operator", 200000, log.log) ]]},
	{"ActivityManager", "goingToSleep", nil, [[ log.add("Operator:", "Operator", 200000, log.log) ]]},
	{"ActivityManager", "START u0 ", nil, [[ log.add("Operator:", "Operator", 200000, log.log) ]]},
	{"ActivityManager", nil, [[ string.find(log.msg, "ACT") == 1 ]] , [[ log.add("Operator:", "Operator", 200000, log.log) ]]},
--	{"ViewRootImpl", "App handle pointer event", nil, [[ log.add("Operator:", "Operator", 200000, log.log) ]]},
--	{"WifiController", "DeviceActiveState", [[ (tonumber(log.values["what"])) > 10 ]], [[ log.add("数据:" .. log.pid) ]]},
--	{"ActivityManager", "SEND MESSAGE", nil, [[ log.add( "SEND:" .. log.pid, log.values["name"], 100) ]]},
--	{"ActivityManager", "GET MESSAGE", nil, [[ log.remove(log.values["name"]) ]]},
--	{"ContextImpl", "MTK", nil, [[ log.add("STACK ERROR", log.values["name"], 3, log.msg) ]]},
--	{"ERR", nil, nil,[[ log.addTagPidInfo("ERR" , "ERR" .. log.pid, 100, log.msg) ]]},
--	{nil, nil, [[ log.process[log.pid] == "system_process" ]], [[ log.add("system_process log:" , "E" .. log.pid, 1000, log.log) ]]},
}


reportInfos = {
	{nil, nil, [[ log.level == "E" ]], [[ log.add("Error" , "E" .. log.pid, 100, log.log) ]]},
}











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

log.oldDate = nil
log.getTime = function (date, time)
	local ret = 0;
	if log.oldDate == nil then
		log.oldDate = date
	end

	if log.oldDate ~= date then
		ret = ret + 24 * 60 * 60 * 1000
	end

	local t = string.split(time, ':')
	ret =  ret + tonumber(t[1]) * 60 * 60 * 1000 + tonumber(t[2]) * 60 * 1000 + tonumber(t[3]) * 1000

	return ret
end

log.dataReps = {}
log.keyReps = {}

log.add = function (commit, key, time, rep)
	if key == nil then 
		key = "TEMP"
	end
	if time == nil then 
		time = 0
	end
	if rep == nil then 
		rep = ""
	end

	if log.keyReps[key] == nil or log.dataReps[log.keyReps[key]] == nil then
		log.dataReps[#log.dataReps + 1] = {log.timeMs + time, log.log, rep .. "\n", commit}
		log.keyReps[key] = #log.dataReps
	else
		local dataReps = log.dataReps[log.keyReps[key]]
		local logLine = dataReps[DLOG]
		local drep = dataReps[DREP]
		if rep ~= "" then
			drep = drep ..  rep .. "\n"
		end
		local commit = dataReps[DCOMMIT]
		local dtime = dataReps[DTIME]

		if dtime < log.timeMs + time then
			dtime = log.timeMs + time
		end

		log.dataReps[log.keyReps[key]] = {dtime, logLine, drep, commit}

	end
end

log.remove = function (key)
	if log.keyReps[key] ~= nil then
		log.dataReps[log.keyReps[key]] = nil
		log.keyReps[key] = nil
	end
end

log.process = {}

log.setPid = function(pid, processName)
	if log.process[pid] == nil then
		log.process[pid] = processName
		log.add("processName", "processName", 1000000, pid  .. "\t: " .. processName)
	end
end 


log.infos = {}

log.addTagPidInfo = function (commit, key, time, rep)
	log.addInfo(log.tag, nil, "log.pid == " .. log.pid,  "log.add(" .. commit .. "," .. key .. "," .. "0" .. "," .. rep .. ")", time)
	log.add(commit, key, time, rep)
end

log.addInfo = function (tag, msg, cond, post, time)
	log.infos[#log.infos + 1] = {tag, msg, cond, post, log.timeMs + time}
end




log.report = function(dataRep)
	local r = ""
	r = r .. "=========================================================================\n"
	-- r = r .. dataRep[DLOG] .. "\n"
	r = r .. dataRep[DCOMMIT] .. "\n"
	r = r .. dataRep[DREP] .. "\n"
	return r
end

log.parser = function (logLines, infos) 
	local r = ""
	for k, logLine in pairs(logLines) do
		log.log = logLine
		log.date, log.time, log.pid, log.tid, log.level, log.tag, log.msg = log.split(logLine)
		if log.time ~= nil then
			log.values = log.getValue(log.msg)
			log.timeMs = log.getTime(log.date, log.time)

			for dk, dataRep in pairs(log.dataReps) do
				if dataRep[DTIME] <= log.timeMs then
					r = r .. log.report(dataRep)
					log.dataReps[dk] = nil
				end
			end



			for tk, info in pairs(infos) do
				if info[TAG] == nil or log.tag == info[TAG] then
					if info[MSG] == nil or string.find(log.msg, info[MSG]) ~= nil then
						if info[COND] == nil or loadstring("return " .. info[COND])() == true then
							if info[POST] ~= nil then
								loadstring(info[POST])()
							end
						end
					end
				end
			end

			for tk, info in pairs(log.infos) do
				if info[TAG] == nil or log.tag == info[TAG] then
					if info[MSG] == nil or string.find(log.msg, info[MSG]) ~= nil then
						if info[COND] == nil or loadstring("return " .. info[COND])() == true then
							if info[POST] ~= nil then
								loadstring(info[POST])()
							end
						end
					end
				end
				if info[TIME] ~= nil and info[TIME] < log.timeMs then
					infos[tk] = nil
				end
			end

		end
	end

	for dk, dataRep in pairs(log.dataReps) do
		r = r .. log.report(dataRep)
		log.dataReps[dk] = nil
	end	

	return r
end



DTIME = 1
DLOG = 2
DREP = 3
DCOMMIT = 4

TAG = 1
MSG = 2
COND = 3
POST = 4
TIME = 5


local logs = string.gsub(notepad.getCurText(), "\r", "")

local logLines = string.split(logs, "\n")

local r

r = log.parser(logLines, processInfos)

r = r .. log.parser(logLines, flowInfos)

r = r .. log.parser(logLines, reportInfos)


notepad.setNewText(r)






