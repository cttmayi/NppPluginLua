//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//#include "targetver.h"
#include "PluginDefinition.h"
#include "PluginUtil.h"
#include "menuCmdID.h"

#define lua_c

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

//
// The plugin data that Notepad++ needs
//
int nbFunc = 0;

FuncItem funcItem[MaxFunc];

TCHAR funcLuaName[MaxFunc][MAX_PATH];


//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

TCHAR luaScriptPath[MAX_PATH];

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{


	//logcatInit();
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
	::SendMessage( nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)luaScriptPath );
	wcscat(luaScriptPath, TEXT("\\plugins\\LuaScript\\"));

	doLuaInit();
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (!pFunc)
	{
        return false;
	}

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}


////////////////////////////////

extern "C" int getCurText(lua_State* L)
{
	HWND curScintilla = nppData._scintillaMainHandle;

	unsigned int len = ::SendMessage(curScintilla, SCI_GETTEXTLENGTH, 0, 0) + 1;

	char *chText = new char[len];
	::SendMessage(curScintilla, SCI_GETTEXT, len, (LPARAM)chText);

	lua_pushstring(L, chText);

	return 1;
}

extern "C" int setCurText(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);

	HWND curScintilla = nppData._scintillaMainHandle;
	::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM) op1);

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

extern "C" int setLuaCommandEx(lua_State* L)
{
	const char* op1 = luaL_checkstring(L,1);
	const char* op2 = luaL_checkstring(L,2);

	TCHAR opt1[MAX_PATH], opt2[MAX_PATH];

	CharToTchar(op1, opt1);
	CharToTchar(op2, opt2);

	//logcatLog(op1);

	setLuaCommand(opt1, opt2, 0);
	return 1;
}

static luaL_Reg NotepadLibs[] = {
	{"getCurText", getCurText},
	{"setCurText", setCurText},
	{"setNewText", setNewText},
	{"openNewFile", openNewFile},
	{"setLuaCommand", setLuaCommandEx},
	{NULL, NULL} 
}; 
/////////////////////////////////////////////////////////


void doLuaFile(TCHAR* fileName)
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

    luaL_dofile(L, fileNameChar);
    lua_close(L);
}

void doLuaScriptFlie(TCHAR* fileName)
{
	TCHAR file[MAX_PATH];
	wcscpy (file, luaScriptPath);
	wcscat(file, fileName);

	doLuaFile(file);


}

void doLuaInit()
{
	doLuaScriptFlie(TEXT("init.lua"));
}



#define DEF_DO_LUA_COMMAND(id) void doLuaCommand_##id() { doLuaScriptFlie(funcLuaName[id]);}
#define DO_LUA_COMMAND(id) doLuaCommand_##id

DEF_DO_LUA_COMMAND(0)
DEF_DO_LUA_COMMAND(1)
DEF_DO_LUA_COMMAND(2)
DEF_DO_LUA_COMMAND(3)
DEF_DO_LUA_COMMAND(4)
DEF_DO_LUA_COMMAND(5)
DEF_DO_LUA_COMMAND(6)
DEF_DO_LUA_COMMAND(7)
DEF_DO_LUA_COMMAND(8)
DEF_DO_LUA_COMMAND(9)

PFUNCPLUGINCMD LuaCommands[MaxFunc] = {
	DO_LUA_COMMAND(0),
	DO_LUA_COMMAND(1),
	DO_LUA_COMMAND(2),
	DO_LUA_COMMAND(3),
	DO_LUA_COMMAND(4),
	DO_LUA_COMMAND(5),
	DO_LUA_COMMAND(6),
	DO_LUA_COMMAND(7),
	DO_LUA_COMMAND(8),
	DO_LUA_COMMAND(9),
};


bool setLuaCommand(TCHAR *cmdName, TCHAR* luaFileName, UCHAR shortcutKey)
{
	ShortcutKey sk;
	ShortcutKey* psk = NULL;
	
    if (nbFunc >= MaxFunc)
        return false;

	if (shortcutKey != 0)
	{
		sk._isAlt = true;
		sk._isCtrl = false;
		sk._isShift = false;
		sk._key = shortcutKey;
		psk = &sk;
	}

	wcscpy(funcLuaName[nbFunc], luaFileName);

	setCommand(nbFunc, cmdName, doLuaCommand, NULL, false);
	nbFunc ++;
	return true;
}