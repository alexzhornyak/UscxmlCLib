#pragma once

struct lua_State;

namespace lua_table_utils {

	extern const char *GetBuffer(unsigned long &nSize);
	extern int LoadBuffer(lua_State *L);

}
