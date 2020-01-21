#pragma once

#include "uscxml/Interpreter.h"
#include "uscxml/interpreter/InterpreterMonitor.h"
#include "uscxml/interpreter/InterpreterImpl.h"
#include "uscxml/plugins/ioprocessor/scxml/SCXMLIOProcessor.h"
#include "uscxml/plugins/ExecutableContentImpl.h"
#include <uscxml/plugins/datamodel/lua/LuaDataModel.h>

class ScxmlBase;

namespace uscxml {

	class LuaDataModelEx : public LuaDataModel {
		ScxmlBase *_ScxmlBase;
	public:
		LuaDataModelEx(ScxmlBase *AScxmlBase) :_ScxmlBase(AScxmlBase) {};

		virtual std::shared_ptr<DataModelImpl> create(DataModelCallbacks* callbacks) override;

		virtual void setup() override;

		virtual std::list<std::string> getNames() {
			std::list<std::string> names;
			names.push_back("luavia");
			return names;
		}

		// Data GetGlobal(string ScxmlName, [opt] Path) 
		static int GetGlobal(lua_State * L);

		// Data GetGlobalDef(string ScxmlName, Data def, [opt] Path)
		static int GetGlobalDef(lua_State *L);

		// SetGlobal(string ScxmlName, Data val, [opt] Type, [opt] Path)
		// Type = 0 
		static int SetGlobal(lua_State * L);

		static int Log(lua_State * L);

		// double GetMicroClock()
		static int GetMicroClock(lua_State * L);

		static int GetAppMicroClock(lua_State * L);
	};

}

