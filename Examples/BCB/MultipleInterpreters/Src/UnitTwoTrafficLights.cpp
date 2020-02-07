//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitTwoTrafficLights.h"

#include <IOUtils.hpp>
#include <IdSync.hpp>

#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTrafficLight *FormTrafficLight = NULL;

UsclibInterpreter *g_Interpreter = NULL;
UsclibInterpreter *g_Interpreter2 = NULL;

//---------------------------------------------------------------------------
// Interpreter 1
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
// Interpreter 2
//---------------------------------------------------------------------------
class TEnterExitNotify2 : public TIdNotify {
	const UnicodeString FStateName;
	const bool FEnter;

protected:
	virtual void __fastcall DoNotify(void) {
		if (FStateName.Pos("red") == 1) {
			FormTrafficLight->ShapeRed2->Brush->Color = FEnter ? clRed : clGray;
		}
		else if (FStateName == "yellow" || FStateName == "blinking") {
			FormTrafficLight->ShapeYellow2->Brush->Color = FEnter ? clYellow : clGray;
		}
		else if (FStateName == "green") {
			FormTrafficLight->ShapeGreen2->Brush->Color = FEnter ? clGreen : clGray;
		}
	}

public:
	__fastcall TEnterExitNotify2(const UnicodeString &sStateName, const bool bEnter) : FStateName(sStateName), FEnter(bEnter) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterEnterExit2(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName,
	const bool bEnter, void *AUser) {
	(new TEnterExitNotify2(chStateName, bEnter))->Notify();
}

//---------------------------------------------------------------------------
//--------------------------- TFormTrafficLight -----------------------------
//---------------------------------------------------------------------------
__fastcall TFormTrafficLight::TFormTrafficLight(TComponent* Owner) : TForm(Owner) {
	try {
		const AnsiString sScxmlFile = TPath::Combine(ExtractFilePath(Application->ExeName),
			"..\\..\\..\\..\\StateCharts\\TrafficLight.scxml");

		/* 1 interpreter */
		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, 0))
			throw Exception(usclib_GetLastError());

		usclib_RegisterInterpreterEnterCallback(g_Interpreter, OnInterpreterEnterExit, this);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, sScxmlFile.c_str(), USCLIB_SCXML_AS_FILE))
			throw Exception(usclib_GetLastError());

		/* 2 interpreter */
		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter2, 0, 0, 0))
			throw Exception(usclib_GetLastError());

		usclib_RegisterInterpreterEnterCallback(g_Interpreter2, OnInterpreterEnterExit2, this);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter2, sScxmlFile.c_str(), USCLIB_SCXML_AS_FILE))
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
	if (g_Interpreter2) {
		usclib_CloseInterpreter(g_Interpreter2);
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
void __fastcall TFormTrafficLight::CheckNightMode2Click(TObject *Sender) {
	if (g_Interpreter2) {
		usclib_TriggerEvent(g_Interpreter2, "switch");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormTrafficLight::CheckPause1Click(TObject *Sender) {
	if (g_Interpreter) {
		CheckPause1->Checked ? usclib_PauseInterpreter(g_Interpreter) : usclib_ResumeInterpreter(g_Interpreter);
		CheckNightMode->Enabled = !CheckPause1->Checked;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormTrafficLight::CheckPause2Click(TObject *Sender) {
	if (g_Interpreter2) {
		CheckPause2->Checked ? usclib_PauseInterpreter(g_Interpreter2) : usclib_ResumeInterpreter(g_Interpreter2);
		CheckNightMode2->Enabled = !CheckPause2->Checked;
	}
}
//---------------------------------------------------------------------------
