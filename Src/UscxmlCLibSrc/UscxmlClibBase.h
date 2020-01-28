#pragma once

#include <set>
#include <unordered_map>
#include <thread>
#include <shared_mutex>

#include <boost/asio.hpp>

#include "uscxml/util/DOM.h"
#include "uscxml/Interpreter.h"
#include "uscxml/interpreter/InterpreterImpl.h"
#include "uscxml/debug/InterpreterIssue.h"

#include "AppTimer.h"
#include "UscxmlCLibCallbacks.h"
#include "UscxmlCLibPausableQueue.h"
#include "UscxmlClibExtensions.h"

#define SCXMLTASK_TO_GLOBAL_ONLY				0

#define SCXML_DISABLE_REMOTE_MONITOR			0
#define SCXML_DEFAULT_SCXMLEDITOR_MONITOR		11005

#define SCXML_HTTP_LISTEN_PORT					7080
#define SCXML_HTTP_WEBSOCKET_PORT				7443

using namespace uscxml;

using boost::asio::ip::udp;

typedef enum {
	smttUnknown, smttAfterEnter, smttBeforeEnter, smttAfterExit, smttBeforeExit, smttStep, smttBeforeExecContent, smttAfterExecContent, 
	smttBeforeInvoke, smttAfterInvoke, smttBeforeUnInvoke, smttAfterUnInvoke, smttBeforeTakingTransition, smttAfterTakingTransition, smttMAXSIZE
} TScxmlMsgType;

static std::string ScxmlMsgTypeToString(const TScxmlMsgType AType) {
	switch (AType) {
	case smttAfterEnter: return "smttAfterEnter";
	case smttBeforeEnter: return "smttBeforeEnter";
	case smttAfterExit: return "smttAfterExit";
	case smttBeforeExit: return "smttBeforeExit";
	case smttBeforeExecContent: return "smttBeforeExecContent";
	case smttAfterExecContent: return "smttAfterExecContent";
	case smttBeforeInvoke: return "smttBeforeInvoke";
	case smttAfterInvoke: return "smttAfterInvoke";
	case smttBeforeUnInvoke: return "smttBeforeUnInvoke";
	case smttAfterUnInvoke: return "smttAfterUnInvoke";
	case smttBeforeTakingTransition: return "smttBeforeTakingTransition";
	case smttAfterTakingTransition: return "smttAfterTakingTransition";
	case smttStep: return "smttStep";
	case smttMAXSIZE: return "smttMAXSIZE";
	}
	return "smttUnknown";
}

/************************************************************************/
/*                       SequenceCheckingMonitor                        */
/************************************************************************/

class ScxmlBase;

class SequenceCheckingMonitor : public uscxml::StateTransitionMonitor {
	boost::asio::io_service _io_service;
	udp::socket _out_socket;
	udp::endpoint _endpoint;

	ScxmlBase *_ScxmlBase;

public:
	SequenceCheckingMonitor(ScxmlBase *AScxmlBase, const Logger &logger);

	virtual void beforeExitingState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state) override;
	virtual void afterExitingState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state) override;

	virtual void beforeExecutingContent(Interpreter& interpreter, const XERCESC_NS::DOMElement* execContent) override;
	virtual void afterExecutingContent(Interpreter& interpreter, const XERCESC_NS::DOMElement* execContent) override;

	virtual void beforeUninvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid) override;
	virtual void afterUninvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid) override;

	virtual void beforeTakingTransition(Interpreter& interpreter, const XERCESC_NS::DOMElement* transition) override;
	virtual void afterTakingTransition(Interpreter& interpreter, const XERCESC_NS::DOMElement* transition) override;

	virtual void beforeEnteringState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state) override;
	virtual void afterEnteringState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state) override;

	virtual void beforeInvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid) override;
	virtual void afterInvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid) override;

	virtual void onStableConfiguration(Interpreter& interpreter) override {}
	virtual void beforeMicroStep(Interpreter& interpreter) override {}

	virtual void beforeProcessingEvent(Interpreter& interpreter, const uscxml::Event& event) override;

	// sends message to remote listener
	void sendMessage(const std::string &sInterpreterName, const std::string &sMsg, const TScxmlMsgType AType);
};


/************************************************************************/
/*                            ScxmlBase                                 */
/************************************************************************/

typedef struct {						
	bool Monitor = true;
	std::string RemoteMonitorHost = "127.0.0.1";
	int  RemoteMonitorPort = SCXML_DISABLE_REMOTE_MONITOR;
	bool Validate = true;
	bool TerminateNotValidated = true;
	std::set<TScxmlMsgType>  MonitorMsgTypes{ smttBeforeEnter,smttBeforeExit, smttBeforeTakingTransition,smttBeforeInvoke, smttBeforeUnInvoke };				// USCLIB_SCXML_EDITOR_MSG_TYPES
	bool DisableGlobalData = false;
	bool AsyncStart = false;
} ScxmlBaseOptions;

class ScxmlBase {
public:

	ScxmlBase(const std::vector<std::string> &ACMDArgs,	const ScxmlBaseOptions &AScxmlBaseOptions, const bool bHttpEnabled = false);

	~ScxmlBase(void);

	inline void registerOnInterpreterLog(OnInterpreterLog AOnInterpreterLog, void *AUser) { 
		_OnInterpreterLog = AOnInterpreterLog;
		_OnInterpreterLogUser = AUser;
	}

	inline void registerOnStopped(OnInterpreterNotify AOnInterpreterStopped, void *AUser) {
		_OnInterpreterStopped = AOnInterpreterStopped;
		_OnInterpreterStoppedUser = AUser;
	}

	inline void registerOnEnter(OnInterpreterEnterExit AOnInterpreterEnter, void *AUser) {
		_OnInterpreterEnterExit = AOnInterpreterEnter;
		_OnInterpreterEnterExitUser = AUser;
	}

	inline void registerOnInvoke(OnInterpreterInvoke AOnInterpreterInvoke, void *AUser) {
		_OnInterpreterInvoke = AOnInterpreterInvoke;
		_OnInterpreterInvokeUser = AUser;
	}

	inline void registerOnEvent(OnInterpreterEvent AOnInterpreterEvent, const bool bAtomOrJson, void *AUser) {
		_OnInterpreterEvent = AOnInterpreterEvent;
		_OnInterpreterEventUser = AUser;
		_OnInterpreterEventAtomOrJson = bAtomOrJson;
	}

	inline void registerOnGlobalDataChange(OnInterpreterGlobalDataChange AOnInterpreterGlobalDataChange, const bool bAtomOrJson, void *AUser) {
		_OnInterpreterGlobalDataChange = AOnInterpreterGlobalDataChange;
		_OnInterpreterGlobalDataChangeUser = AUser;
		_OnInterpreterGlobalDataChangeAtomOrJson = bAtomOrJson;
	}
	
	void start(const std::string &sTextOrFile, const bool bIsText);	
	void pause();
	void resume();
	void close();
	void waitForStopped(void);
	void receive(const Event& event);

	inline bool useRemoteMonitor(void) const { return _options.RemoteMonitorPort != SCXML_DISABLE_REMOTE_MONITOR; }

	InterpreterState getState() const;
	inline InterpreterImpl *getImpl() const { return _interpreter ? _interpreter.getImpl().get() : nullptr; }

	/* methods for extended Lua DataModel (luavia) */
	uscxml::Data getGlobal(const std::string &sScxmlName, const std::string &sPath) const;
	void setGlobal(const std::string &sScxmlName, const std::string &sPath, const uscxml::Data &data, const int iType);
	bool isGlobalDataEnabled() const { return !_options.DisableGlobalData; }

	const std::string getExeDir() const;
	std::string getProjectPath() const;
	inline const std::string &getProjectFileName() const { return _scxmlurl; }
	inline const std::vector<std::string> &getCMDArgs() { return _CMDArgs; }	
	inline const AppTimer &getAppTimer() const { return _AppTimer; }

protected:

	/* Options */
	const std::vector<std::string> _CMDArgs;

	const ScxmlBaseOptions _options;
	const bool _httpEnabled;

	/* CALLBACKS */
	friend class SequenceCheckingMonitor;

	OnInterpreterLog _OnInterpreterLog = nullptr;
	void *_OnInterpreterLogUser = nullptr;

	OnInterpreterEnterExit _OnInterpreterEnterExit = nullptr;
	void *_OnInterpreterEnterExitUser = nullptr;

	OnInterpreterInvoke _OnInterpreterInvoke = nullptr;
	void *_OnInterpreterInvokeUser = nullptr;

	OnInterpreterEvent _OnInterpreterEvent = nullptr;
	void *_OnInterpreterEventUser = nullptr;
	bool _OnInterpreterEventAtomOrJson = false;

	OnInterpreterNotify _OnInterpreterStopped = nullptr;
	void *_OnInterpreterStoppedUser = nullptr;

	OnInterpreterGlobalDataChange _OnInterpreterGlobalDataChange = nullptr;
	void *_OnInterpreterGlobalDataChangeUser = nullptr;
	bool _OnInterpreterGlobalDataChangeAtomOrJson = false;

	/* Pausable Queue */
	friend class PausableDelayedEventQueue;

	std::shared_ptr<uscxml::PausableDelayedEventQueue> _queue_ptr;
	std::list<std::weak_ptr<PausableDelayedEventQueue>> _nestedDelayQueue;

private:
	
	ScxmlBase(const ScxmlBase&) = delete;
	ScxmlBase& operator=(const ScxmlBase&) = delete;
	
	std::string _scxmlurl = "";

	/* for global data exchange */
	mutable std::shared_mutex _mutex;

	InterpreterState _state = InterpreterState::USCXML_UNDEF;
	Interpreter _interpreter;	
	ActionLanguage _lang;
	std::unique_ptr<FactoryDynamic> _factory_ptr;	
	std::unique_ptr<SequenceCheckingMonitor> _monitor_ptr;
	std::thread * _interpreter_thread_ptr = nullptr; // do not use smart pointer! AccessViolation in C++ Builder

	void setState(InterpreterState AState);
	void blockingQueue();	
	
	/* Performance Extensions */
	std::unordered_map<std::string /* ScxmlName */, uscxml::Data> _global_data_storage;
	AppTimer _AppTimer;
};

