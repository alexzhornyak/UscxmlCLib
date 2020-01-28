#include "UscxmlLuaDataModelEx.h"

#include <LuaBridge.h>
#include "LuaTableUtils.h"
#include "UscxmlClibLogger.h"
#include "UscxmlClibBase.h"

namespace uscxml {

	std::shared_ptr<DataModelImpl> LuaDataModelEx::create(DataModelCallbacks* callbacks) {

		std::shared_ptr<LuaDataModelEx> dm(new LuaDataModelEx(_ScxmlBase));
		dm->_callbacks = callbacks;
		dm->setup();

		return dm;
	}

	void LuaDataModelEx::setup(void) {
		LuaDataModel::setup();

		if (lua_table_utils::LoadBuffer(_luaState))
		{
			std::string s_error(lua_tostring(_luaState, -1));
			lua_pop(_luaState, 1);
			CLOG(ERROR) << "Can not execute [lua_table_utils::LoadBuffer] with msg:" << s_error << std::endl;
		}


		lua_pushcfunction(_luaState, SetGlobal);
		lua_setglobal(_luaState, "SetGlobal");

		lua_pushcfunction(_luaState, GetGlobal);
		lua_setglobal(_luaState, "GetGlobal");

		lua_pushcfunction(_luaState, GetGlobalDef);
		lua_setglobal(_luaState, "GetGlobalDef");

		lua_pushcfunction(_luaState, Log);
		lua_setglobal(_luaState, "Log");

		lua_pushcfunction(_luaState, GetMicroClock);
		lua_setglobal(_luaState, "GetMicroClock");

		lua_pushcfunction(_luaState, GetAppMicroClock);
		lua_setglobal(_luaState, "GetAppMicroClock");

		lua_pushlightuserdata(_luaState, _ScxmlBase);
		lua_setglobal(_luaState, "_ScxmlBase");

		luabridge::setGlobal(_luaState, static_cast<int>(SysLogSeverity::slDEBUG), "DEBUG");
		luabridge::setGlobal(_luaState, static_cast<int>(SysLogSeverity::slINFO), "INFO");
		luabridge::setGlobal(_luaState, static_cast<int>(SysLogSeverity::slWARN), "WARNING");
		luabridge::setGlobal(_luaState, static_cast<int>(SysLogSeverity::slERROR), "ERROR");
		luabridge::setGlobal(_luaState, static_cast<int>(SysLogSeverity::slCRITICAL), "CRITICAL");

		luabridge::setGlobal(_luaState, _ScxmlBase->getExeDir(), "s_EXE_DIR");

		luabridge::setGlobal(_luaState, _ScxmlBase->getProjectPath(), "s_PROJECT_PATH");
		luabridge::setGlobal(_luaState, _ScxmlBase->getProjectPath(), "s_PROJECT_DIR");
		luabridge::setGlobal(_luaState, _ScxmlBase->getProjectFileName(), "s_PROJECT_FILENAME");

		luabridge::setGlobal(_luaState, SCXMLTASK_TO_GLOBAL_ONLY, "TO_GLOBAL_ONLY");
		luabridge::setGlobal(_luaState, SCXMLTASK_TO_PROTOCOL, "TO_PROTOCOL");

		luabridge::LuaRef ACMDArgs = luabridge::newTable(_luaState);
		for (std::size_t n = 0; n < _ScxmlBase->getCMDArgs().size(); n++) {
			ACMDArgs[n + 1] = _ScxmlBase->getCMDArgs()[n];
		}

		luabridge::setGlobal(_luaState, ACMDArgs, "t_CMD_ARGS");
	}


	int LuaDataModelEx::GetGlobal(lua_State * L)
	{
		const char *chName = luaL_checkstring(L, 1);

		const char *chPath = luaL_optstring(L, 2, "");

		lua_getglobal(L, "_ScxmlBase");
		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase *>(lua_touserdata(L, -1));
		lua_pop(L, 1);

		uscxml::Data data = AScxmlBase->getGlobal(chName, chPath);

		luabridge::push(L, LuaDataModel::getDataAsLua(L, data));

		return 1;
	}

	int LuaDataModelEx::GetGlobalDef(lua_State * L)
	{
		const char *chName = luaL_checkstring(L, 1);

		luabridge::LuaRef default = luabridge::LuaRef::fromStack(L, 2);

		const char *chPath = luaL_optstring(L, 3, "");

		lua_getglobal(L, "_ScxmlBase");
		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase *>(lua_touserdata(L, -1));
		lua_pop(L, 1);

		uscxml::Data data = AScxmlBase->getGlobal(chName, chPath);

		if (!data.empty()) {
			luabridge::push(L, LuaDataModel::getDataAsLua(L, data));
		}
		else {
			luabridge::push(L, default);
		}

		return 1;
	}

	int LuaDataModelEx::SetGlobal(lua_State * L)
	{
		const int preStack = lua_gettop(L);
		if (preStack >= 2)
		{
			const char *chName = luaL_checkstring(L, 1);

			const uscxml::Data &data = LuaDataModel::getLuaAsData(L, luabridge::LuaRef::fromStack(L, 2));

			const int iType = luaL_optinteger(L, 3, 0);

			const char *chPath = luaL_optstring(L, 4, "");

			lua_getglobal(L, "_ScxmlBase");
			ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase *>(lua_touserdata(L, -1));
			lua_pop(L, 1);

			AScxmlBase->setGlobal(chName, chPath, data, iType);
		}
		else {
			CLOG(ERROR) << "[SetGlobal] Requires at least 2 params, but got <" << preStack << ">" << std::endl;
		}

		return 0;
	}

	int LuaDataModelEx::Log(lua_State * L)
	{
		const int preStack = lua_gettop(L);
		if (preStack == 2)
		{
			const std::size_t iLevel = luaL_checkinteger(L, 1);
			const char *chText = luaL_checkstring(L, 2);

			switch (iLevel) {
			case SysLogSeverity::slDEBUG:
				CLOG(DEBUG) << chText << std::endl;
				break;
			case SysLogSeverity::slINFO:
				CLOG(INFO) << chText << std::endl;
				break;
			case SysLogSeverity::slWARN:
				CLOG(WARN) << chText << std::endl;
				break;
			case SysLogSeverity::slERROR:
				CLOG(ERROR) << chText << std::endl;
				break;
			case SysLogSeverity::slCRITICAL:
				CLOG(CRITICAL) << chText << std::endl;
			}
		}
		else {
			CLOG(ERROR) << __FUNCTION__ << "> Requires 2 params, but got <" << preStack << ">" << std::endl;
		}

		return 0;
	}

	int LuaDataModelEx::GetMicroClock(lua_State * L)
	{
		auto tm = std::chrono::high_resolution_clock::now();

		auto micro = std::chrono::time_point_cast<std::chrono::microseconds>(tm);

		lua_pushnumber(L, micro.time_since_epoch().count());

		return 1;
	}

	int LuaDataModelEx::GetAppMicroClock(lua_State * L)
	{
		lua_getglobal(L, "_ScxmlBase");
		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase *>(lua_touserdata(L, -1));
		lua_pop(L, 1);

		using namespace std::chrono;

		lua_pushnumber(L, duration_cast<microseconds>(AScxmlBase->getAppTimer().getTime()).count());

		return 1;
	}

}