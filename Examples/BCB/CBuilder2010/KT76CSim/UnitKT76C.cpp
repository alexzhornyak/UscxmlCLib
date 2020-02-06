//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitKT76C.h"

#include <IOUtils.hpp>
#include <IdSync.hpp>

#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKT76C *FormKT76C = NULL;

UsclibInterpreter *g_Interpreter = NULL;

//---------------------------------------------------------------------------
class TGlobalDataNotify : public TIdNotify {
	const UnicodeString FName;
	const UnicodeString FData;

protected:
	virtual void __fastcall DoNotify(void) {
		for (int i = 0; i < FormKT76C->ComponentCount; i++) {
			if (TLabel * ALabel = dynamic_cast<TLabel*>(FormKT76C->Components[i])) {
				if (ALabel->Hint == FName) {

                    // On-Off controls were marked with 'Tag=1'

					switch(ALabel->Tag) {
					case 1:
						ALabel->Visible = FData.ToIntDef(0);
						break;
					default:
						ALabel->Caption = FData;
					}
				}
			}
		}
	}

public:
	__fastcall TGlobalDataNotify(const UnicodeString &sName, const UnicodeString &sData) : FName(sName), FData(sData) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterGlobalDataChange(const UsclibInterpreter *AInterpreter, const char *chName, const char *chPath,
	const char *chAtomOrJsonData, const bool bIsAtomOrJson, const int iType, void *AUser) {

	// Type = 0 - global data changed from App
	// Type = 1 - global data changed inside Scxml

	if (iType == 1) {
		(new TGlobalDataNotify(chName, chAtomOrJsonData))->Notify();
	}
}

//---------------------------------------------------------------------------
//--------------------------- TFormTrafficLight -----------------------------
//---------------------------------------------------------------------------
__fastcall TFormKT76C::TFormKT76C(TComponent* Owner) : TForm(Owner) {
	try {
		const AnsiString sScxmlFile = TPath::Combine(ExtractFilePath(Application->ExeName), "..\\..\\..\\..\\StateCharts\\KT76C.scxml");

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

		usclib_RegisterInterpreterGlobalDataChangeCallback(g_Interpreter, OnInterpreterGlobalDataChange, USCLIB_DATATYPE_ATOM, NULL);

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

//---------------------------------------------------------------------------
__fastcall TFormKT76C::~TFormKT76C(void) {
	if (g_Interpreter) {
        // we generate 'Quit' event to give State Chart an option to save settings
		usclib_TriggerEvent(g_Interpreter, "Quit");
		usclib_WaitForInterpreterStopped(g_Interpreter);

		usclib_CloseInterpreter(g_Interpreter);
	}
	usclib_GlobalCleanup();
}

//---------------------------------------------------------------------------

void __fastcall TFormKT76C::BtnSupplyClick(TObject *Sender) {
	if (g_Interpreter) {
		const AnsiString sName = BtnSupply->Hint;
		usclib_TriggerIntEvent(g_Interpreter, sName.c_str(), BtnSupply->Down ? 1 : 0);
		BtnSupply->Font->Color = BtnSupply->Down ? clMaroon : clWindowText;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormKT76C::TrackBar1Change(TObject *Sender) {
	if (g_Interpreter) {
		const AnsiString sName = TrackBar1->Hint;
		usclib_TriggerIntEvent(g_Interpreter, sName.c_str(), TrackBar1->Position);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormKT76C::SpeedButton1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TSpeedButton * AButton = dynamic_cast<TSpeedButton*>(Sender);
	if (AButton) {
		if (!AButton->Tag) {
			AButton->Tag = 1;
			AButton->Font->Color = clMaroon;

			const AnsiString sName = AButton->Hint;
			usclib_TriggerIntEvent(g_Interpreter, sName.c_str(), 1);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKT76C::SpeedButton1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TSpeedButton * AButton = dynamic_cast<TSpeedButton*>(Sender);
	if (AButton) {
		if (AButton->Tag && !Shift.Contains(ssCtrl)) {
			AButton->Tag = 0;
			AButton->Font->Color = clWindowText;

			const AnsiString sName = AButton->Hint;
			usclib_TriggerIntEvent(g_Interpreter, sName.c_str(), 0);
		}
	}
}
//---------------------------------------------------------------------------
