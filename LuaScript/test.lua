require "notepad"

str = notepad.getCurText()

-- notepad.setNewText(str)

--notepad.gotoLine(10)

--notepad.showDialog("curline",notepad.getCurLineText())
str = notepad.getSelText()

if notepad.getLine() == 9 then
	str = "ABC"
end

notepad.setStyleColor(15, util.rgb(255, 0, 0), 0)
notepad.setStyle(1, 18, 15)


str = notepad.getLineText(notepad.getLine())

str = notepad.getFileName()
notepad.setNewText(str)
--notepad.showDialog("sel",str)