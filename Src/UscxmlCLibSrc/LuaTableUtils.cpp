#include "LuaTableUtils.h"

#include <Lua.hpp>

namespace lua_table_utils {

static const unsigned char B[]={
	 27, 76,117, 97, 83,  0, 25,147, 13, 10, 26, 10,  4,  4,  4,  8,  8,120, 86,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 40,119, 64,  1,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  1,  5,  9,  0,  0,  0, 44,  0,  0,  0,108, 64,  0,  0,172,128,
	  0,  0,236,192,  0,  0, 44,  1,  1,  0,  8,  0,  1,128, 44, 65,  1,  0,  8,  0,
	129,128, 38,  0,128,  0,  2,  0,  0,  0,  4, 16, 83, 97,118,101, 84, 97, 98,108,
	101, 84,111, 70,105,108,101,  4, 14, 68,101,101,112, 67,111,112,121, 84, 97, 98,
	108,101,  1,  0,  0,  0,  1,  0,  6,  0,  0,  0,  0,  1,  0,  0,  0,  8,  0,  0,
	  0,  1,  0,  9, 18,  0,  0,  0, 75,  0,  0,  0,134,  0, 64,  0,192,  0,  0,  0,
	164,  0,  1,  1, 30,  0,  1,128,134, 65, 64,  0,135,129, 64,  3,192,  1,128,  0,
	  0,  2,128,  2,164, 65,128,  1,169, 64,  0,  0, 42,  1,254,127,134, 64, 64,  0,
	135,192, 64,  1,192,  0,128,  0,164, 64,  0,  1,102,  0,  0,  1, 38,  0,128,  0,
	  4,  0,  0,  0,  4,  6,112, 97,105,114,115,  4,  6,116, 97, 98,108,101,  4,  7,
	105,110,115,101,114,116,  4,  5,115,111,114,116,  1,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0, 37,
	  0,  0,  0,  2,  0,  8, 35,  0,  0,  0,  8, 64, 64,128, 31, 64,192,  0, 30,192,
	  1,128,133,  0,128,  0,192,  0,  0,  0,164,128,  0,  1, 10,128,  0,129,135,128,
	 64,  0,135,192, 64,  1,  8,128,  0,128, 30, 64,  3,128,129,192,  0,  0,199,128,
	 64,  0,220,  0,128,  1,  1,193,  0,  0,168,192,  1,128,135,129, 64,  0,135, 65,
	  1,  3, 31, 64,  0,  3, 30,192,  0,128,135,129, 64,  0,205,193,192,  2,135,193,
	  1,  3,  8,128,  1,128,167,128,253,127,134,  0, 64,  0,162,  0,  0,  0, 30,192,
	  0,128,134,  0, 64,  0,198,  0, 64,  0,199,192,  0,  0,166,  0,128,  1, 10, 64,
	 64,129, 38,  0,128,  0, 38,  0,128,  0,  4,  0,  0,  0,  4,  4,107,101,121,  0,
	  4, 15, 95, 95,111,114,100,101,114,101,100, 73,110,100,101,120, 19,  1,  0,  0,
	  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 39,  0,  0,  0, 43,  0,  0,  0,  1,  0,
	  4,  5,  0,  0,  0, 69,  0,  0,  0,128,  0,  0,  0,196,  0,  0,  0,102,  0,  0,
	  2, 38,  0,128,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 45,  0,  0,  0, 84,  0,  0,  0,
	  3,  0, 16,159,  0,  0,  0,162, 64,  0,  0, 30, 64,  0,128,203,  0,  0,  0,128,
	  0,128,  1, 98, 64,  0,  0, 30,  0,  0,128, 65,  0,  0,  0,198, 64, 64,  0,  0,
	  1,  0,  0,228,128,  0,  1, 31,128,192,  1, 30, 64, 35,128,203,  0,  0,  0,  5,
	  1,128,  0, 64,  1,  0,  0, 36,  1,  1,  1, 30,  0, 32,128, 70,130, 64,  0, 71,
	194,192,  4,128,  2,128,  1,198,  2, 65,  0,199, 66,193,  5,  1,131,  1,  0, 64,
	  3,128,  0,228,  2,128,  1,100, 66,  0,  0, 70, 66, 64,  0,128,  2,  0,  4,100,
	130,  0,  1, 31,128,192,  4, 30,128, 12,128, 71,  2,  2,  1, 98, 66,  0,  0, 30,
	192, 11,128,138,192, 65,  4, 70,  2, 66,  0,128,  2,128,  3,100,130,  0,  1,129,
	 66,  2,  0, 93,130,130,  4,134, 66, 64,  0,192,  2,128,  3,164,130,  0,  1, 31,
	128, 66,  5, 30, 64,  1,128,129,194,  2,  0,198,  2, 66,  0,  0,  3,128,  3,228,
	130,  0,  1,  1,  3,  3,  0, 93,  2,  3,  5,134,130, 64,  0,135,194, 64,  5,192,
	  2,128,  1,  0,  3,128,  4, 65, 67,  3,  0, 29, 67,  3,  6,164, 66,128,  1,134,
	130, 64,  0,135,194, 64,  5,192,  2,128,  1,  5,  3,  0,  1, 64,  3,  0,  4,141,
	131,195,  0,192,  3,  0,  1, 36,  3,  0,  2,164, 66,  0,  0,134,130, 64,  0,135,
	194, 64,  5,192,  2,128,  1,  6,  3, 65,  0,  7, 67, 65,  6, 65,131,  1,  0,128,
	  3,128,  0, 36,  3,128,  1,164, 66,  0,  0,134,130, 64,  0,135,194, 64,  5,192,
	  2,128,  1,  1,195,  3,  0,164, 66,128,  1, 30,192, 15,128, 70, 66, 64,  0,128,
	  2,128,  3,100,130,  0,  1, 31, 64,  2,133, 30,128,  6,128, 70, 66, 64,  0,128,
	  2,  0,  4,100,130,  0,  1, 31,  0,193,  4, 30, 64,  2,128, 70,130, 64,  0, 71,
	194,192,  4,128,  2,128,  1,198,  2, 65,  0,199,  2,196,  5,  1, 67,  4,  0, 64,
	  3,  0,  4,228,  2,128,  1,100, 66,  0,  0, 30,192, 10,128, 70,130, 64,  0, 71,
	194,192,  4,128,  2,128,  1,198,  2, 65,  0,199,  2,196,  5,  1,131,  4,  0, 70,
	  3, 66,  0,128,  3,  0,  4,100,  3,  0,  1,228,  2,  0,  0,100, 66,  0,  0, 30,
	192,  7,128, 70, 66, 64,  0,128,  2,  0,  4,100,130,  0,  1, 31,  0,193,  4, 30,
	  0,  3,128, 70,130, 64,  0, 71,194,192,  4,128,  2,128,  1,198,  2, 65,  0,199,
	  2,196,  5,  1,195,  4,  0, 70,  3, 66,  0,128,  3,128,  3,100,131,  0,  1,128,
	  3,  0,  4,228,  2,  0,  2,100, 66,  0,  0, 30, 64,  3,128, 70,130, 64,  0, 71,
	194,192,  4,128,  2,128,  1,198,  2, 65,  0,199,  2,196,  5,  1,  3,  5,  0, 70,
	  3, 66,  0,128,  3,128,  3,100,131,  0,  1,134,  3, 66,  0,192,  3,  0,  4,164,
	  3,  0,  1,228,  2,  0,  0,100, 66,  0,  0, 41,129,  0,  0,170,  1,223,127,  6,
	129, 64,  0,  7, 65, 69,  2, 64,  1,128,  1, 37,  1,  0,  1, 38,  1,  0,  0, 30,
	192,  0,128,192,  0,  0,  0,  1,129,  5,  0,221,  0,129,  1,230,  0,  0,  1, 38,
	  0,128,  0, 23,  0,  0,  0, 19,  0,  0,  0,  0,  0,  0,  0,  0,  4,  5,116,121,
	112,101,  4,  6,116, 97, 98,108,101,  4,  7,105,110,115,101,114,116,  4,  7,115,
	116,114,105,110,103,  4,  4,114,101,112,  4,  2, 32,  1,  1,  4,  9,116,111,115,
	116,114,105,110,103,  4,  2, 61,  4,  7,110,117,109, 98,101,114,  4,  2, 91,  4,
	  3, 93, 61,  4,  3,123, 10, 19,  2,  0,  0,  0,  0,  0,  0,  0,  4,  4,125, 59,
	 10,  4,  7,102,111,114,109, 97,116,  4,  5, 37,113, 59, 10,  4,  5, 37,115, 59,
	 10,  4, 10, 37,115, 32, 61, 32, 37,113, 59, 10,  4, 10, 37,115, 32, 61, 32, 37,
	115, 59, 10,  4,  7, 99,111,110, 99, 97,116,  4,  2, 10,  3,  0,  0,  0,  0,  0,
	  1,  2,  1,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0, 86,  0,  0,  0, 95,  0,  0,  0,  3,  0, 12, 28,  0,  0,  0,198,  0, 64,  0,
	199, 64,192,  1,  0,  1,  0,  0, 65,129,  0,  0,228,128,128,  1, 95,192,192,  1,
	 30,192,  4,128,162,  0,  0,  0, 30,128,  0,128,  1,  1,  1,  0, 34, 65,  0,  0,
	 30,  0,  0,128,  1, 65,  1,  0, 76,129,193,  1,192,  1,  0,  2,  1,194,  1,  0,
	 69,  2,128,  0,128,  2,128,  0,193,  2,  2,  0,100,130,128,  1,129, 66,  2,  0,
	221,129,130,  3,100, 65,128,  1, 70,  1, 64,  0, 71,129,194,  2,128,  1,128,  1,
	100, 65,  0,  1, 38,  0,128,  0, 11,  0,  0,  0,  4,  3,105,111,  4,  5,111,112,
	101,110,  4,  3,119, 43,  0,  4,  8,114,101,116,117,114,110, 32,  4,  1,  4,  6,
	119,114,105,116,101,  4,  3,123, 10, 19,  2,  0,  0,  0,  0,  0,  0,  0,  4,  2,
	125,  4,  6, 99,108,111,115,101,  2,  0,  0,  0,  0,  0,  1,  3,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 97,  0,  0,  0,110,  0,  0,
	  0,  1,  0, 11, 33,  0,  0,  0, 70,  0, 64,  0,128,  0,  0,  0,100,128,  0,  1,
	132,  0,  0,  0, 31, 64,192,  0, 30,192,  5,128,203,  0,  0,  0,128,  0,128,  1,
	198,128, 64,  0,  0,  1,  0,  0, 68,  1,  0,  0, 30,128,  1,128,  6,194, 64,  0,
	 64,  2,  0,  3, 36,130,  0,  1, 70,194, 64,  0,128,  2,128,  3,100,130,  0,  1,
	138, 64,  2,  4,233,128,  0,  0,106,129,253,127,198,  0, 65,  0,  0,  1,  0,  1,
	 70,193, 64,  0,134, 65, 65,  0,192,  1,  0,  0,164,  1,  0,  1,100,  1,  0,  0,
	228, 64,  0,  0, 30,  0,  0,128,128,  0,  0,  0,166,  0,  0,  1, 38,  0,128,  0,
	  6,  0,  0,  0,  4,  5,116,121,112,101,  4,  6,116, 97, 98,108,101,  4,  5,110,
	101,120,116,  4, 14, 68,101,101,112, 67,111,112,121, 84, 97, 98,108,101,  4, 13,
	115,101,116,109,101,116, 97,116, 97, 98,108,101,  4, 13,103,101,116,109,101,116,
	 97,116, 97, 98,108,101,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

};


	const char * GetBuffer(unsigned long &nSize) { nSize = sizeof(B);	return reinterpret_cast<const char*>(B); }

	const char *getS (lua_State *L, void *ud, size_t *size) { *size = sizeof(B); return reinterpret_cast<const char*>(B); }

	int LoadBuffer(lua_State *L) { return lua_load(L,getS,(void*)B,"lua_table_utils","b")||lua_pcall(L,0,LUA_MULTRET,0); }

}