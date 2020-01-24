#pragma once

#include "uscxml/Interpreter.h"
#include "uscxml/interpreter/InterpreterImpl.h"
#include "uscxml/plugins/ioprocessor/scxml/SCXMLIOProcessor.h"
#include "uscxml/plugins/ExecutableContentImpl.h"

class ScxmlBase;

namespace uscxml {

	class GlobalDataIOProcessor : public SCXMLIOProcessor {
		ScxmlBase *_ScxmlBase;
	public:
		GlobalDataIOProcessor(ScxmlBase *AScxmlBase);

		virtual std::list<std::string> getNames() override {
			std::list<std::string> names;
			names.push_back("global_data");
			return names;
		};

		virtual std::shared_ptr<IOProcessorImpl> create(IOProcessorCallbacks* callbacks) override;

		virtual void eventFromSCXML(const std::string& target, const Event& event) override;

	};

	class SetValueExecutableContent : public uscxml::ExecutableContentImpl {
		ScxmlBase *_ScxmlBase;
	public:
		SetValueExecutableContent(ScxmlBase *AScxmlBase);

		virtual std::shared_ptr<ExecutableContentImpl> create(uscxml::InterpreterImpl* interpreter) override {
			std::shared_ptr<SetValueExecutableContent> content_ptr(new SetValueExecutableContent(_ScxmlBase));
			content_ptr->setInterpreter(interpreter);
			return content_ptr;
		}

		virtual std::string getLocalName() override { return "setvalue"; }

		virtual void enterElement(XERCESC_NS::DOMElement* node) override;

		virtual void exitElement(XERCESC_NS::DOMElement* node) override;

		virtual bool processChildren() override { return false; };

	};

	/* destructor is hidden in base 'Factory' class, so making this trick */
	class FactoryDynamic : public uscxml::Factory {
	public:
		FactoryDynamic(uscxml::Factory* parentFactory) :uscxml::Factory(parentFactory) {}
	};
}
