// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitCalculator.h"

#include <memory>
#include <IOUtils.hpp>
#include <IdSync.hpp>

#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCalculator *FormCalculator = NULL;

UsclibInterpreter *g_Interpreter = NULL;

// ---------------------------------------------------------------------------
class TEventNotify : public TIdNotify {
	const UnicodeString FName;
	const UnicodeString FData;

protected:
	virtual void __fastcall DoNotify(void) {
		FormCalculator->LabelDisplay->Caption = FData;
	}

public:
	__fastcall TEventNotify(const UnicodeString &sName, const UnicodeString &sData) : FName(sName), FData(sData) {
	}
};

// ---------------------------------------------------------------------------
void __stdcall OnInterpreterEvent(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chEventName,
	const char *chAtomOrJsonData, const UsclibDataType bIsAtomOrJson, void *AUser) {
	if (SameText(chEventName, "updateDisplay.display")) {
		(new TEventNotify(chEventName, chAtomOrJsonData))->Notify();
	}
}

// ---------------------------------------------------------------------------
// --------------------------- TFormTrafficLight -----------------------------
// ---------------------------------------------------------------------------
__fastcall TFormCalculator::TFormCalculator(TComponent* Owner) : TForm(Owner) {
	try {
		const AnsiString sScxmlFile = TPath::GetFullPath(TPath::Combine(ExtractFilePath(Application->ExeName),
				"..\\..\\..\\..\\StateCharts\\calculator.scxml"));

		const AnsiString sLogFile = ChangeFileExt(Application->ExeName, ".scxml.log");
		if (USCLIB_SUCCESS != usclib_InitLogging(sLogFile.c_str(), NULL, NULL))
			throw Exception(usclib_GetLastError());

		// enable RemoteMonitor
		UsclibInterpreterOptions AInterpreterOptions;
		usclib_GetDefaultInterpreterOptions(&AInterpreterOptions);
		AInterpreterOptions.RemoteMonitorPort = USCLIB_DEFAULT_SCXMLEDITOR_MONITOR;
		AInterpreterOptions.Validate = false;

		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, &AInterpreterOptions))
			throw Exception(usclib_GetLastError());

		usclib_RegisterInterpreterEventCallback(g_Interpreter, OnInterpreterEvent, USCLIB_DATATYPE_ATOM, NULL);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, sScxmlFile.c_str(), USCLIB_SCXML_AS_FILE))
			throw Exception(usclib_GetLastError());
	}
	catch(Exception * E) {
		TrayIcon1->BalloonHint = E->Message;
		TrayIcon1->BalloonFlags = bfError;
		TrayIcon1->ShowBalloonHint();
		TrayIcon1->BalloonTitle = "ERROR";
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormCalculator::~TFormCalculator(void) {
	if (g_Interpreter) {
		usclib_StopInterpreter(g_Interpreter);

		usclib_CloseInterpreter(g_Interpreter);
	}
	usclib_GlobalCleanup();
}

// ---------------------------------------------------------------------------

void __fastcall TFormCalculator::ButtonClick(TObject *Sender) {
	TButton * AButton = dynamic_cast<TButton*>(Sender);
	if (AButton) {
		if (!AButton->Hint.IsEmpty()) {
			const AnsiString sName = AButton->Hint;
			usclib_TriggerEvent(g_Interpreter, sName.c_str());
		}
		else {
			const AnsiString sName = "DIGIT." + AButton->Caption;
			usclib_TriggerEvent(g_Interpreter, sName.c_str());
		}
	}
}
// ---------------------------------------------------------------------------
