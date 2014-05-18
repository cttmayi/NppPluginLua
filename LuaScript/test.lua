require "notepad"

local text = "Message: Hello\nMessage is red color\n\n"

notepad.setCurText(text)



str = notepad.getCurText()

if text ~= str then
	notepad.setNewText("Text error")
	return
end

notepad.gotoLine(2)

if notepad.getLine() ~= 2 then
	notepad.setNewText("Line error")
	return
end

if notepad.getCurLineText() ~= "Message is red color\n" then
	notepad.setNewText("LineText error")
	return
end


notepad.setSel(0, 3)

if notepad.getSelText() ~= "Mes" then
	notepad.setNewText("Sel error")
	return
end

notepad.setStyleColor(15, util.rgb(255, 0, 0), 0)

notepad.setWordStyle("Message", 15)

