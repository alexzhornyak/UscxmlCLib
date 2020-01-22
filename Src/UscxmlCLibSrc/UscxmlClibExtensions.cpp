#include "UscxmlClibExtensions.h"

#include "uscxml/util/DOM.h"
#include <xercesc/dom/DOM.hpp>

#include "UscxmlClibBase.h"

namespace uscxml {

	/* GlobalDataIOProcessor */
	GlobalDataIOProcessor::GlobalDataIOProcessor(ScxmlBase *AScxmlBase) : _ScxmlBase(AScxmlBase) {
	}

	void GlobalDataIOProcessor::eventFromSCXML(const std::string& target, const Event& event) {
		SCXMLIOProcessor::eventFromSCXML(target, event);
		_ScxmlBase->setGlobal(event.name, "", event.data, SCXMLTASK_TO_GLOBAL_ONLY);
	}

	std::shared_ptr<IOProcessorImpl> GlobalDataIOProcessor::create(IOProcessorCallbacks* callbacks) {
		std::shared_ptr<GlobalDataIOProcessor> ioProc(new GlobalDataIOProcessor(_ScxmlBase));
		ioProc->_callbacks = callbacks;
		return ioProc;
	}

	/* SetValueExecutableContent */
	SetValueExecutableContent::SetValueExecutableContent(ScxmlBase *AScxmlBase) :_ScxmlBase(AScxmlBase) {

	}

	void SetValueExecutableContent::enterElement(XERCESC_NS::DOMElement* node) {
		// std::cout << "Entering custom element" << std::endl;
	}
	void SetValueExecutableContent::exitElement(XERCESC_NS::DOMElement* node) {
		// std::cout << "Exiting custom element" << std::endl;

		std::string s_target = "";
		if (HAS_ATTR(node, kXMLCharTarget)) {
			s_target = ATTR(node, kXMLCharTarget);
		}
		else if (HAS_ATTR(node, kXMLCharTargetExpr)) {			
			s_target = _interpreter->evalAsData(ATTR(node, kXMLCharTargetExpr)).atom;
		}
		else {
			ERROR_EXECUTION_THROW2("<setvalue> element has neither 'target' nor 'targetexpr' attribute", node);
		}

		std::string s_type = "0"; // by default SCXMLTASK_TO_GLOBAL_ONLY
		if (HAS_ATTR(node, kXMLCharType)) {
			s_type = ATTR(node, kXMLCharType);
		}
		else if (HAS_ATTR(node, kXMLCharTypeExpr)) {
			s_type = _interpreter->evalAsData(ATTR(node, X("targetexpr"))).atom;
		}

		std::string s_path = "";
		if (HAS_ATTR(node, X("path"))) {
			s_path = ATTR(node, X("path"));
		}
		else if (HAS_ATTR(node, X("pathexpr"))) {
			s_path = _interpreter->evalAsData(ATTR(node, X("pathexpr"))).atom;
		}

		if (HAS_ATTR(node, X("value"))) {
			uscxml::Data data(ATTR(node, X("value")),uscxml::Data::INTERPRETED);
			_ScxmlBase->setGlobal(s_target, s_path, data, std::stoi(s_type));
		}
		else if (HAS_ATTR(node, X("valueexpr"))) {
			uscxml::Data data = _interpreter->evalAsData(ATTR(node, X("valueexpr")));
			_ScxmlBase->setGlobal(s_target, s_path, data, std::stoi(s_type));
		}
		else {
			ERROR_EXECUTION_THROW2("<setvalue> element has neither 'value' nor 'valueexpr' attribute", node);
		}
	}
		
}