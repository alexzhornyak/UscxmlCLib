//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLibraryTest.h"

#include <IdSync.hpp>
#include <IOUtils.hpp>

#include "UnitFrameState0.h"
#include "UnitFrameState1.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormGlobalLibraryTest *FormGlobalLibraryTest = NULL;

//---------------------------------------------------------------------------
class TLogNotify : public TIdNotify {
	const UnicodeString FMessage;
	const int FSeverity;

protected:
	virtual void __fastcall DoNotify(void) {
		FormGlobalLibraryTest->Log(FMessage, FSeverity);
	}

public:
	__fastcall TLogNotify(const UnicodeString &sMsg, const int iSev) : FMessage(sMsg), FSeverity(iSev) {
	}
};

void __stdcall OnGlobalLogCallback(const int nSeverity, const char *chMessage, void *AUser) {
	(new TLogNotify(chMessage, nSeverity))->Notify();
}

//---------------------------------------------------------------------------
class TEnterExitNotify : public TIdNotify {
	const UnicodeString FStateMachineName;
	const UnicodeString FStateName;
	const bool FEnter;

protected:
	virtual void __fastcall DoNotify(void) {
		if (FStateMachineName == "ScxmlGlobalTest") {
			if (FStateName == "On") {

				FormGlobalLibraryTest->PanelInstruments->DisableAlign();
				for (int i = 0; i < FormGlobalLibraryTest->PanelInstruments->ControlCount; i++) {
					if (FormGlobalLibraryTest->PanelInstruments->Controls[i] != FormGlobalLibraryTest->CheckInitTesting) {
						FormGlobalLibraryTest->PanelInstruments->Controls[i]->Visible = FEnter;
					}
				}
				FormGlobalLibraryTest->PanelInstruments->EnableAlign();

				/* last selected on Enter */
				if (FEnter) {
					FormGlobalLibraryTest->ComboModes->OnChange(FormGlobalLibraryTest->ComboModes);
				}

			}
			else if (FStateName == "State0") {
				if (FEnter) {
					FrameState0 = new TFrameState0(FormGlobalLibraryTest);
					FrameState0->Parent = FormGlobalLibraryTest;
					FrameState0->Align = alClient;
				}
				else {
					delete FrameState0;
					FrameState0 = NULL;
				}
			}
			else if (FStateName == "State1") {
				if (FEnter) {
					FrameState1 = new TFrameState1(FormGlobalLibraryTest);
					FrameState1->Parent = FormGlobalLibraryTest;
					FrameState1->Align = alClient;
				}
				else {
					delete FrameState1;
					FrameState1 = NULL;
				}
			}
		}
	}

public:
	__fastcall TEnterExitNotify(const UnicodeString &sStateMachineName, const UnicodeString &sStateName, const bool bEnter)
		: FStateMachineName(sStateMachineName), FStateName(sStateName), FEnter(bEnter) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterEnterExit(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName,
	const bool bEnter, void *AUser) {
	(new TEnterExitNotify(chStateMachineName, chStateName, bEnter))->Notify();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TEventNotify : public TIdNotify {
	const UnicodeString FStateMachineName;
	const UnicodeString FEvent;
	const UnicodeString FData;

protected:
	virtual void __fastcall DoNotify(void) {
		if (FStateMachineName == "ScxmlGlobalTest") {
			if (FEvent == "Out.Data") {
				if (FrameState0) {
					FrameState0->OnDataRespond(FData);
				}
			}
			else if (FEvent == "Out.CyrillicSucceeded") {
				ShowMessage("Cyrillic test SUCCEEDED!\n\n" "P.S. Check also that modal window doesn't affect state machine!");
			}
		}
		else if (FStateMachineName == "ScxmlChild") {
			if (FEvent == "IO.Timer.Signal") {
				if (FrameState1) {
					FrameState1->MemoIOTimerSignal->Text = FEvent + " =" + FData;
				}
			}
		}
	}

public:
	__fastcall TEventNotify(const UnicodeString &sStateMachineName, const UnicodeString &sEvent, const UnicodeString sData)
		: FStateMachineName(sStateMachineName), FEvent(sEvent), FData(sData) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterEvent(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chEventName,
	const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser) {
	(new TEventNotify(chStateMachineName, chEventName, chAtomOrJsonData))->Notify();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TGlobalDataNotify : public TIdNotify {
	const UnicodeString FName;
	const UnicodeString FData;
	const int FType;

protected:
	virtual void __fastcall DoNotify(void) {
		if (FType == 1) {
			if (FName.Pos("Out.Invoke") == 1) {
				if (FrameState1) {
					FrameState1->OnGlobalDataReceived(FName, FData);
				}
			}
			else if (FName.Pos("Out.Main") == 1) {
				for (int i = 0; i < FormGlobalLibraryTest->PanelState2->ControlCount; i++) {
					if (FormGlobalLibraryTest->PanelState2->Controls[i]->Hint == FName) {
						if (TLabel * ALabel = dynamic_cast<TLabel*>(FormGlobalLibraryTest->PanelState2->Controls[i])) {
							ALabel->Caption = FName + "=" + FData;
						}
						else if (TMemo * AMemo = dynamic_cast<TMemo*>(FormGlobalLibraryTest->PanelState2->Controls[i])) {
							AMemo->Text = FName + "=" + FData;
						}
					}
				}
			}
		}
	}

public:
	__fastcall TGlobalDataNotify(const UnicodeString & sName, const UnicodeString sData, const int iType) : FName(sName), FData(sData),
	FType(iType) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterGlobalDataChange(const UsclibInterpreter *AInterpreter, const char *chName, const char *chPath,
	const char *chAtomOrJsonData, const UsclibDataType bIsAtomOrJson, const int iType, void *AUser) {
	(new TGlobalDataNotify(chName, chAtomOrJsonData, iType))->Notify();
}

//---------------------------------------------------------------------------
//--------------------------- TFormHelloWorld -------------------------------
//---------------------------------------------------------------------------
__fastcall TFormGlobalLibraryTest::TFormGlobalLibraryTest(TComponent* Owner) : TForm(Owner), FInterpreter(NULL) {
	usclib_InitLogging(NULL, OnGlobalLogCallback, NULL);
	try {
		const AnsiString sScxmlFile = TPath::Combine(ExtractFilePath(Application->ExeName),
			"..\\..\\..\\..\\StateCharts\\GlobalLibraryTest.scxml");

		const AnsiString sLogFile = ChangeFileExt(Application->ExeName, ".scxml.log");
		if (USCLIB_SUCCESS != usclib_InitLogging(sLogFile.c_str(), NULL, NULL))
			throw Exception(usclib_GetLastError());

		// enable RemoteMonitor
		UsclibInterpreterOptions AInterpreterOptions;
		usclib_GetDefaultInterpreterOptions(&AInterpreterOptions);
		AInterpreterOptions.RemoteMonitorPort = USCLIB_DEFAULT_SCXMLEDITOR_MONITOR;

		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&FInterpreter, 0, 0, &AInterpreterOptions))
			throw Exception(usclib_GetLastError());

		usclib_RegisterInterpreterEnterCallback(FInterpreter, OnInterpreterEnterExit, NULL);

		usclib_RegisterInterpreterEventCallback(FInterpreter, OnInterpreterEvent, USCLIB_DATATYPE_JSON, NULL);

		usclib_RegisterInterpreterGlobalDataChangeCallback(FInterpreter, OnInterpreterGlobalDataChange, USCLIB_DATATYPE_JSON, NULL);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(FInterpreter, sScxmlFile.c_str(), false))
			throw Exception(usclib_GetLastError());
	}
	catch(Exception * E) {
		Log(E->Message, USCLIB_LOG_ERROR);
	}
}

//---------------------------------------------------------------------------
__fastcall TFormGlobalLibraryTest::~TFormGlobalLibraryTest(void) {
	if (FInterpreter) {
		usclib_CloseInterpreter(FInterpreter);
	}
	usclib_GlobalCleanup();
}

//---------------------------------------------------------------------------
void __fastcall TFormGlobalLibraryTest::Log(const UnicodeString & sMsg, const int ASeverity) {
	if (sMsg.IsEmpty())
		return;

	switch(ASeverity) {
	case USCLIB_LOG_INFO: {
			RichEdit1->SelAttributes->Color = clBlue;
			RichEdit1->SelAttributes->Style = TFontStyles();
		}break;
	case USCLIB_LOG_WARN: {
			RichEdit1->SelAttributes->Color = clNavy;
			RichEdit1->SelAttributes->Style = TFontStyles() << fsBold;
		}break;
	case USCLIB_LOG_EMERGENCY:
	case USCLIB_LOG_ALERT:
	case USCLIB_LOG_CRITICAL:
	case USCLIB_LOG_ERROR: {
			RichEdit1->SelAttributes->Color = clRed;
			RichEdit1->SelAttributes->Style = TFontStyles() << fsBold;
		}break;
	default: {
			RichEdit1->SelAttributes->Color = clGreen;
			RichEdit1->SelAttributes->Style = TFontStyles();
		}
	}

	RichEdit1->SelText = sMsg + "\n";
	PostMessage(RichEdit1->Handle, WM_VSCROLL, SB_BOTTOM, 0);
}

//---------------------------------------------------------------------------

void __fastcall TFormGlobalLibraryTest::CheckInitTestingClick(TObject *Sender) {
	if (FInterpreter) {
		usclib_TriggerIntEvent(FInterpreter, "Inp.Init", CheckInitTesting->Checked ? 1 : 0);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormGlobalLibraryTest::ComboModesChange(TObject *Sender) {
	if (FInterpreter) {
		usclib_TriggerIntEvent(FInterpreter, "Inp.Check", ComboModes->ItemIndex);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormGlobalLibraryTest::BtnCheckCyrillicClick(TObject *Sender) {
	if (FInterpreter) {
		const AnsiString sText = L"Тест кириллицы!";
		usclib_TriggerStringEvent(FInterpreter, "Inp.CheckCyrillic", sText.c_str());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormGlobalLibraryTest::CheckPauseClick(TObject *Sender) {
	for (int i = 0; i < PanelInstruments->ControlCount; i++) {
		if (PanelInstruments->Controls[i] != CheckPause) {
			PanelInstruments->Controls[i]->Enabled = !CheckPause->Checked;
		}
	}
	if (FInterpreter) {
		CheckPause->Checked ? usclib_PauseInterpreter(FInterpreter) : usclib_ResumeInterpreter(FInterpreter);
	}
}
//---------------------------------------------------------------------------
