
util = {}

util.rgb = function (r, g, b)
	return (b * 256 + g ) * 256 + r
end

--获取路径  
util.getFileName = function (filename)  
    return string.match(filename, "(.+)\\[^\\]*%.%w+$") --*nix system  
    --return string.match(filename, “(.+)\\[^\\]*%.%w+$”) — windows  
end  
  
--获取文件名  
util.getPath = function (filename)  
    return string.match(filename, ".+\\([^\\]*%.%w+)$") -- *nix system  
    --return string.match(filename, “.+\\([^\\]*%.%w+)$”) — *nix system  
end  
  
--去除扩展名  
util.getFilePath = function (filename)  
    local idx = filename:match(".+()%.%w+$")  
    if(idx) then  
        return filename:sub(1, idx-1)  
    else  
        return filename  
    end  
end  
  
--获取扩展名  
util.getExtension = function (filename)  
    return filename:match(".+%.(%w+)$")  
end  