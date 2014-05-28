
local doc = [[setLuaCommandEx(title, file, key): 添加插件菜单在LuaScript中, 可指定Ctrl+Alt+key的快捷方式
str = getCurText(): 获取当前文件的文本内容
setCurText(str): 设置当前文件的文本内容
setNewText(str): 设置新建文件的文本内容
str = getSelText(): 获取当前选中的文本内容
gotoLine(number): 光标调整到number行





]]

notepad.setNewText(doc)
notepad.showDialog("Help", "Lua Script Document")