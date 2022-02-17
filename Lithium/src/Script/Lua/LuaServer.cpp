#include "lipch.h"
#include "LuaServer.h"
#include "Core/Base.h"

namespace Lithium
{

	LuaServer::LuaServer()
	{
		std::string cmd;
		std::ifstream stream("assets/scripts/test.lua");
		cmd = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
		L = luaL_newstate();
		luaL_openlibs(L);
		int r = luaL_dostring(L, cmd.c_str());
	
		if (r == LUA_OK)
		{
			lua_getglobal(L, "a");

			lua_getfield(L, -1, "OnUpdate");
			lua_pushnumber(L, 5.5f);
			
			lua_pcall(L, 1, 0, 0);
		} 
		else
		{
			std::string errormessage = lua_tostring(L, -1);
			CORE_LOG(errormessage);
		}
	}

}