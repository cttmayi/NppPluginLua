

#include "PluginDefinition.h"
#include "menuCmdID.h"

extern NppData nppData;

HWND logScintilla;

extern "C" void LogcatInit() 
{
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
	logScintilla = nppData._scintillaMainHandle;
}

extern "C" void LogcatLog(char* log)
{
	::SendMessage(logScintilla, SCI_SETTEXT, 0, (LPARAM) log );
}

extern "C" void TcharToChar (const TCHAR * tchar, char * c)  
{
	int len = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);  
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, c, len, NULL, NULL);   
}

extern "C" void CharToTchar(const char* c, TCHAR * tchar)
{  
    int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);  
    MultiByteToWideChar(CP_ACP, 0, c, strlen(c) + 1, tchar , len + 1);
}  

