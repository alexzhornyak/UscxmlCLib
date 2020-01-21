//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitTrafficLight.h"

#include <IOUtils.hpp>
#include <IdSync.hpp>

#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTrafficLight *FormTrafficLight = NULL;

UsclibInterpreter *g_Interpreter = NULL;

//---------------------------------------------------------------------------
class TEnterExitNotify : public TIdNotify {
	const UnicodeString FStateName;
	const bool FEnter;

protected:
	virtual void __fastcall DoNotify(void) {
		if (FStateName.Pos("red") == 1) {
			FormTrafficLight->ShapeRed->Brush->Color = FEnter ? clRed : clGray;
		}
		else if (FStateName == "yellow" || FStateName == "blinking") {
			FormTrafficLight->ShapeYellow->Brush->Color = FEnter ? clYellow : clGray;
		}
		else if (FStateName == "green") {
			FormTrafficLight->ShapeGreen->Brush->Color = FEnter ? clGreen : clGray;
		}
	}

public:
	__fastcall TEnterExitNotify(const UnicodeString &sStateName, const bool bEnter) : FStateName(sStateName), FEnter(bEnter) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterEnterExit(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName,
	const bool bEnter, void *AUser) {
	(new TEnterExitNotify(chStateName, bEnter))->Notify();
}

//---------------------------------------------------------------------------
//--------------------------- TFormTrafficLight -----------------------------
//---------------------------------------------------------------------------
__fastcall TFormTrafficLight::TFormTrafficLight(TComponent* Owner) : TForm(Owner) {
	try {
		const AnsiString sScxmlFile = TPath::Combine(ExtractFilePath(Application->ExeName),
			"..\\..\\..\\..\\StateCharts\\TrafficLight.scxml");

		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, 0))
			throw Exception(usclib_GetLastError());

		usclib_RegisterInterpreterEnterCallback(g_Interpreter, OnInterpreterEnterExit, this);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, sScxmlFile.c_str(), false))
			throw Exception(usclib_GetLastError());
	}
	catch(Exception * E) {
		TrayIcon1->BalloonHint = E->Message;
		TrayIcon1->BalloonFlags = bfError;
		TrayIcon1->ShowBalloonHint();
		TrayIcon1->BalloonTitle = "ERROR";
	}
}

//---------------------------------------------------------------------------
__fastcall TFormTrafficLight::~TFormTrafficLight(void) {
	if (g_Interpreter) {
		usclib_CloseInterpreter(g_Interpreter);
	}
	usclib_GlobalCleanup();
}

//---------------------------------------------------------------------------
void __fastcall TFormTrafficLight::CheckNightModeClick(TObject *Sender) {
	if (g_Interpreter) {
		usclib_TriggerEvent(g_Interpreter, "switch");
	}
}
//---------------------------------------------------------------------------
