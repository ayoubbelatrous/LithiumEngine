#pragma once
#include "lua.hpp"


namespace Lithium
{
	class LuaServer
	{
	public:

		LuaServer();
	private:
		lua_State* L;
	};
}