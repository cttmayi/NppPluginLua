#define lua_c



#include "PluginDefinition.h"
#include "PluginUtil.h"
#include "menuCmdID.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

extern NppData nppData;

////////////////////////////////

extern "C" int getText(lua_State* L, long lenMsg, long textMsg)
{
	HWND curScintilla = nppData._scintillaMainHandle;

	unsigned int len = ::SendMessage(curScintilla, lenMsg, 0, 0) + 1;

	char *chText = new char[len];
	::SendMessage(curScintilla, textMsg, len, (LPARAM)chText);

	lua_pushstring(L, chText);

	return 1;
}


////////////////////////////////

extern "C" int getCurText(lua_State* L)
{
	return getText(L, SCI_GETTEXTLENGTH, SCI_GETTEXT);
}


// fix me !!!
extern "C" int getSelText(lua_State* L)
{
	HWND curScintilla = nppData._scintillaMainHandle;

	unsigned int len = ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, 0) + 1;

	char *chText = new char[len];
	::SendMessage(curScintilla, SCI_GETSELTEXT, len, (LPARAM)chText);

	chText[len] = 0;

	lua_pushstring(L, chText);

	return 1;
}

extern "C" int getCurLineText(lua_State* L)
{
	return getText(L, SCI_GETCURLINE, SCI_GETCURLINE);
}

extern "C" int setCurText(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);

	HWND curScintilla = nppData._scintillaMainHandle;
	::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM) op1);

	return 1;
}

extern "C" int appendCurText(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);

	HWND curScintilla = nppData._scintillaMainHandle;
	::SendMessage(curScintilla, SCI_APPENDTEXT, 0, (LPARAM) op1);

	return 1;
}

extern "C" int setNewText(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);

    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
	HWND newScintilla = nppData._scintillaMainHandle;
	::SendMessage(newScintilla, SCI_SETTEXT, 0, (LPARAM) op1 );

	return 1;
}

extern "C" int openNewFile(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);

	::SendMessage(NULL, NPPM_DOOPEN, 0, (LPARAM) op1 );

	return 1;
}

extern "C" int gotoLine(lua_State* L)
{
	int line = luaL_checkint(L,1) - 1;

	HWND curScintilla = nppData._scintillaMainHandle;
	::SendMessage(curScintilla, SCI_GOTOLINE, line, 0 );

	return 1;
}

extern "C" int setLuaCommandEx(lua_State* L)
{
	size_t op3Len;
	const char* op1 = luaL_checkstring(L,1);
	const char* op2 = luaL_checkstring(L,2);
	const char* op3 = luaL_tolstring(L,3, &op3Len);

	TCHAR opt1[MAX_PATH];

	CharToTchar(op1, opt1);

	if (op3 == NULL || op3Len == 0)
	{ 
		setLuaCommand(opt1, op2, 0);
	}
	else
	{
		setLuaCommand(opt1, op2, op3[0]);
	}
	return 1;
}
extern "C" int showDialog(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);
	const char* op2 = luaL_checkstring(L,2);

#define MAX_TITLE 40
#define MAX_MSG 400


	TCHAR opt1[MAX_TITLE+1]; 
	TCHAR opt2[MAX_MSG+1];

	if (strlen(op1) > MAX_TITLE || strlen(op2) > MAX_MSG )
	{
		return 1;
	}

	CharToTchar(op1, opt1);
	CharToTchar(op2, opt2);

	::MessageBox(NULL, opt2, opt1, MB_OK);
	return 1;
}

static luaL_Reg NotepadLibs[] = {
	{"setLuaCommand", setLuaCommandEx},
	{"showDialog", showDialog},
	{"getCurText", getCurText},
	{"getSelText", getSelText},
	{"getCurLineText", getCurLineText},
	{"setCurText", setCurText},
	{"appendCurText", appendCurText},
	{"setNewText", setNewText},
	{"gotoLine", gotoLine},
	{"openNewFile", openNewFile},
	{NULL, NULL} 
}; 
/////////////////////////////////////////////////////////


void doLuaFile(const TCHAR* fileName, const char* luaCommand)
{
	lua_State *L = luaL_newstate();  /* create state */
	if (L == NULL) {
		//l_message("lua", "cannot create state: not enough memory");
		//return EXIT_FAILURE;
		return;
	}
    luaL_openlibs(L);

	luaL_register(L, "notepad", NotepadLibs);

	char fileNameChar[MAX_PATH];

	TcharToChar(fileName, fileNameChar);

	if (luaCommand != NULL)
	{
		luaL_dostring(L, luaCommand);
	}

    luaL_dofile(L, fileNameChar);

    lua_close(L);
}