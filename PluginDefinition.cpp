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



//
// The plugin data that Notepad++ needs
//
int nbFunc = 0;

FuncItem funcItem[MaxFunc];

char funcLuaName[MaxFunc][MAX_PATH];

ShortcutKey funcLuaSck[MaxFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

TCHAR luaScriptPath[MAX_PATH];
char luaScriptPathChar[MAX_PATH];

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{


	
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
	LogcatInit();
	::SendMessage( nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)luaScriptPath );
	wcscat(luaScriptPath, TEXT("\\plugins\\LuaScript\\"));

	TcharToChar(luaScriptPath, luaScriptPathChar);

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
bool setCommand(size_t index, const TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
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




void doLuaScriptFlie(char* fileName)
{
	TCHAR file[MAX_PATH];
	wcscpy (file, luaScriptPath);
	wcscat(file, TEXT("lib//plugin.lua"));

	char luaCommand[MAX_PATH + 20];
	strcpy(luaCommand, "luaFile = [[");
	strcat(luaCommand, luaScriptPathChar);
	strcat(luaCommand, fileName);
	strcat(luaCommand, "]]");
	doLuaFile(file, luaCommand);


}

void doLuaInit()
{
	TCHAR file[MAX_PATH];
	wcscpy (file, luaScriptPath);
	wcscat(file, TEXT("init.lua"));

	doLuaFile(file, NULL);
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
DEF_DO_LUA_COMMAND(10)
DEF_DO_LUA_COMMAND(11)
DEF_DO_LUA_COMMAND(12)
DEF_DO_LUA_COMMAND(13)
DEF_DO_LUA_COMMAND(14)
DEF_DO_LUA_COMMAND(15)
DEF_DO_LUA_COMMAND(16)
DEF_DO_LUA_COMMAND(17)
DEF_DO_LUA_COMMAND(18)
DEF_DO_LUA_COMMAND(19)

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
	DO_LUA_COMMAND(10),
	DO_LUA_COMMAND(11),
	DO_LUA_COMMAND(12),
	DO_LUA_COMMAND(13),
	DO_LUA_COMMAND(14),
	DO_LUA_COMMAND(15),
	DO_LUA_COMMAND(16),
	DO_LUA_COMMAND(17),
	DO_LUA_COMMAND(18),
	DO_LUA_COMMAND(19),
};


bool setLuaCommand(const TCHAR *cmdName, const char* luaFileName, UCHAR shortcutKey)
{
	//ShortcutKey sk;
	ShortcutKey* psk = NULL;
	
    if (nbFunc >= MaxFunc)
        return false;

	if (shortcutKey != 0)
	{
		funcLuaSck[nbFunc]._isAlt = true;
		funcLuaSck[nbFunc]._isCtrl = true;
		funcLuaSck[nbFunc]._isShift = false;
		funcLuaSck[nbFunc]._key = shortcutKey;
		psk = funcLuaSck+nbFunc;
	}

	strcpy(funcLuaName[nbFunc], luaFileName);
	//wcscpy(funcLuaName[nbFunc], luaFileName);

	setCommand(nbFunc, cmdName, LuaCommands[nbFunc], psk, false);
	nbFunc ++;
	return true;
}