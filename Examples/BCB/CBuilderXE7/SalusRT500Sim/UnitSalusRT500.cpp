//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSalusRT500.h"

#include <IOUtils.hpp>
#include <PngImage.hpp>
#include "UnitInterpreterCallbacks.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFormSalusRT500 *FormSalusRT500 = NULL;

const int g_BTN_PRESSED_OFFSET = 2;

bool g_Started = false;

//---------------------------------------------------------------------------
//--------------------------- TFormTrafficLight -----------------------------
//---------------------------------------------------------------------------
__fastcall TFormSalusRT500::TFormSalusRT500(TComponent* Owner) : TForm(Owner), FInterpreter(NULL) {
	try {

		Model.reset(new SalusDataModel);

		for (int i = 0; i < frameOutputs->ControlCount; i++) {
			TControl * AControl = dynamic_cast<TControl*>(frameOutputs->Controls[i]);
			if (AControl) {
				AControl->Visible = false;
			}
		}

		for (int i = 0; i < this->ComponentCount; i++) {
			TImage * AImage = dynamic_cast<TImage*>(this->Components[i]);
			if (AImage && AImage->Name.Pos("Img") == 1) {
				std::auto_ptr<TPngImage>APNGImage(new TPngImage());
				APNGImage->LoadFromResourceName(reinterpret_cast<unsigned>(HInstance),
					StringReplace(AImage->Name, "Img", "Png", TReplaceFlags()).c_str());
				AImage->Picture->Assign(APNGImage.get());
			}
		}

		const AnsiString sScxmlFile = TPath::Combine(ExtractFilePath(Application->ExeName),
			"..\\..\\..\\..\\StateCharts\\SalusRT500Logic.scxml");

		const AnsiString sLogFile = ChangeFileExt(Application->ExeName, ".scxml.log");
		if (USCLIB_SUCCESS != usclib_InitLogging(sLogFile.c_str(), NULL, NULL))
			throw Exception(usclib_GetLastError());

		// enable RemoteMonitor
		UsclibInterpreterOptions AInterpreterOptions;
		usclib_GetDefaultInterpreterOptions(&AInterpreterOptions);
		AInterpreterOptions.RemoteMonitorPort = USCLIB_DEFAULT_SCXMLEDITOR_MONITOR;
		AInterpreterOptions.Validate = false;

		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&FInterpreter, 0, 0, &AInterpreterOptions))
			throw Exception(usclib_GetLastError());

		usclib_RegisterInterpreterEnterCallback(FInterpreter, OnInterpreterEnterExit, NULL);
		usclib_RegisterInterpreterEventCallback(FInterpreter, OnInterpreterEvent, USCLIB_DATATYPE_ATOM, NULL);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(FInterpreter, sScxmlFile.c_str(), USCLIB_SCXML_AS_FILE))
			throw Exception(usclib_GetLastError());

		g_Started = true;
	}
	catch(Exception * E) {
		ShowBalloon(E->Message, bfError);
	}
}

//---------------------------------------------------------------------------
__fastcall TFormSalusRT500::~TFormSalusRT500(void) {
	g_Closing = true;

	if (FInterpreter) {
		usclib_CloseInterpreter(FInterpreter);
	}

	usclib_GlobalCleanup();
}

//---------------------------------------------------------------------------
void TFormSalusRT500::ShowBalloon(const UnicodeString &sMsg, const TBalloonFlags AFlags) {
	TrayIcon1->BalloonHint = sMsg;
	TrayIcon1->BalloonFlags = AFlags;
	TrayIcon1->BalloonTitle = "";
	switch(AFlags) {
	case bfInfo:
		TrayIcon1->BalloonTitle = "INFO";
		break;
	case bfError:
		TrayIcon1->BalloonTitle = "ERROR";
		break;
	}
	TrayIcon1->ShowBalloonHint();
}

//---------------------------------------------------------------------------
void __fastcall TFormSalusRT500::BtnMouseDown(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TImage * AImage = dynamic_cast<TImage*>(Sender);
	if (AImage) {

		if (!AImage->Tag) {
			AImage->Tag = 1;

			AImage->Left = AImage->Left + g_BTN_PRESSED_OFFSET;
			AImage->Top = AImage->Top + g_BTN_PRESSED_OFFSET;

			if (g_Started) {
				const AnsiString sName = StringReplace(AImage->Name, "Img", "Inp.Btn", TReplaceFlags());
				usclib_TriggerIntEvent(FInterpreter, sName.c_str(), 1);
			}
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSalusRT500::BtnMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TImage * AImage = dynamic_cast<TImage*>(Sender);
	if (AImage) {
		if (AImage->Tag && !Shift.Contains(ssCtrl)) {
			AImage->Tag = 0;

			AImage->Left = AImage->Left - g_BTN_PRESSED_OFFSET;
			AImage->Top = AImage->Top - g_BTN_PRESSED_OFFSET;

			if (g_Started) {
				const AnsiString sName = StringReplace(AImage->Name, "Img", "Inp.Btn", TReplaceFlags());
				usclib_TriggerIntEvent(FInterpreter, sName.c_str(), 0);
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSalusRT500::CheckPowerClick(TObject * Sender) {
	if (g_Started) {
		usclib_TriggerIntEvent(FInterpreter, "CheckPower", CheckPower->Checked ? 1 : 0);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSalusRT500::SpinTemperatureChange(TObject *Sender) {
	if (g_Started) {
		usclib_TriggerIntEvent(FInterpreter, "SpinTemperature", SpinTemperature->Value);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSalusRT500::TimerFlameTimer(TObject *Sender) {
	ImgFlame->Visible = TimerFlame->Tag;
	TimerFlame->Tag = !TimerFlame->Tag;
}

//---------------------------------------------------------------------------
void __fastcall TFormSalusRT500::TimerFreezeTimer(TObject *Sender) {
	ImgSnow->Visible = TimerFreeze->Tag;
	TimerFreeze->Tag = !TimerFreeze->Tag;
}

//---------------------------------------------------------------------------
void __fastcall TFormSalusRT500::FileOpenProgramAccept(TObject *Sender) {
	try {
		Model->loadFromFile(FileOpenProgram->Dialog->FileName);
	}
	catch(Exception * E) {
		ShowBalloon(E->Message, bfError);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormSalusRT500::FileSaveProgramAccept(TObject *Sender) {
	try {
		Model->saveToFile(FileSaveProgram->Dialog->FileName);
	}
	catch(Exception * E) {
		ShowBalloon(E->Message, bfError);
	}

}
//---------------------------------------------------------------------------
