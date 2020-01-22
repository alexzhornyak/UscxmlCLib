//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitTesterW3C.h"

#include <memory>

#include <IOUtils.hpp>
#include <IdSync.hpp>

#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormW3C *FormW3C = NULL;

UsclibInterpreter *g_Interpreter = NULL;

#define SCXML_TREE_NODE_SUCCESS		0
#define SCXML_TREE_NODE_ERROR		2
#define SCXML_TREE_NODE_STARTED		4
#define SCXML_TREE_NODE_UNKNOWN		6
#define SCXML_TREE_NODE_WARNING		8

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TStoppedNotify : public TIdNotify {
	TTreeNode *FNode;
	bool FPass;

protected:
	virtual void __fastcall DoNotify(void) {

		const bool bUserCancelled = !FormW3C->BtnStop->Enabled;

		if (!bUserCancelled) {
			if (FNode->Data) {
				FNode->ImageIndex = SCXML_TREE_NODE_WARNING;
			}
			else {
				FNode->ImageIndex = FPass ? SCXML_TREE_NODE_SUCCESS : SCXML_TREE_NODE_ERROR;
			}
			FNode->SelectedIndex = FNode->ImageIndex;
			FNode->MakeVisible();

			FormW3C->ProgressBar1->Position = FNode->Index + 1;
			FormW3C->InterpreterStartNext();
		}

	}

public:
	__fastcall TStoppedNotify(TTreeNode *ANode, const bool bPass) : FNode(ANode), FPass(bPass) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterStopped(const UsclibInterpreter *AInterpreter, void *AUser) {
	bool bPass = false;
	usclib_IsInterpreterInState(AInterpreter, "pass", &bPass);
	(new TStoppedNotify(reinterpret_cast<TTreeNode*>(AUser), bPass))->Notify();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TLogNotify : public TIdNotify {
	const UnicodeString FMessage;
	const int FSeverity;

protected:
	virtual void __fastcall DoNotify(void) {
		FormW3C->Log(FMessage, FSeverity);
	}

public:
	__fastcall TLogNotify(const UnicodeString &sMsg, const int iSeverity) : FMessage(sMsg), FSeverity(iSeverity) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterLog(const UsclibInterpreter *AInterpreter, const int nSeverity, const char *chMessage, void *AUser) {
	(new TLogNotify(chMessage, nSeverity))->Notify();
}

//---------------------------------------------------------------------------
//--------------------------- TFormTrafficLight -----------------------------
//---------------------------------------------------------------------------
__fastcall TFormW3C::TFormW3C(TComponent* Owner) : TForm(Owner) {
	try {
		// manual or restricted tests
		std::auto_ptr<TStringList>AWarningTestsPtr(new TStringList());
		AWarningTestsPtr->Add("test230.scxml"); // manual
		AWarningTestsPtr->Add("test250.scxml"); // manual
		AWarningTestsPtr->Add("test301.scxml"); // rejected script src
		AWarningTestsPtr->Add("test307.scxml"); // manual
		AWarningTestsPtr->Add("test415.scxml"); // halt processing when final is initial
		AWarningTestsPtr->Add("test530.scxml"); // XML Nodes in script expressions not supported
		AWarningTestsPtr->Add("test562.scxml"); // content without evaluation not supported (only valid expressions)

		FDirW3C = TPath::Combine(ExtractFilePath(Application->ExeName), "..\\..\\..\\..\\StateCharts\\W3C\\Lua");

		if (!DirectoryExists(FDirW3C))
			throw Exception("Directory [" + FDirW3C + "] does not exist!");

		const AnsiString sLogFile = ChangeFileExt(Application->ExeName, ".scxml.log");
		if (USCLIB_SUCCESS != usclib_InitLogging(sLogFile.c_str()))
			throw Exception(usclib_GetLastError());

		if (USCLIB_SUCCESS != usclib_InitHTTP(7080, 7443))
			throw Exception(usclib_GetLastError());

		TStringDynArray AFiles = TDirectory::GetFiles(FDirW3C, "*.scxml");
		for (int i = 0; i < AFiles.Length; i++) {
			if (!AFiles[i].Pos("sub")) {

				TTreeNode *ANode = TreeTests->Items->AddChild(NULL, ExtractFileName(AFiles[i]));

				ANode->ImageIndex = SCXML_TREE_NODE_UNKNOWN;
				ANode->SelectedIndex = ANode->ImageIndex;

				if (AWarningTestsPtr->IndexOf(ANode->Text) != -1) {
					ANode->Data = reinterpret_cast<void*>(1);
				}
			}
		}

		ProgressBar1->Max = TreeTests->Items->Count;

		BtnStart->Enabled = true;
	}
	catch(Exception * E) {
		Log("ERROR> " + E->Message, USCLIB_LOG_ERROR);
	}
}

//---------------------------------------------------------------------------
__fastcall TFormW3C::~TFormW3C(void) {
	if (g_Interpreter) {
		usclib_CloseInterpreter(g_Interpreter);
	}

	usclib_GlobalCleanup();
}

//---------------------------------------------------------------------------
void __fastcall TFormW3C::Log(const UnicodeString & sMsg, const int ASeverity) {
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
void __fastcall TFormW3C::InterpreterStartNext() {
	try {
		for (int i = 0; i < TreeTests->Items->Count; i++) {
			if (TreeTests->Items->Item[i]->ImageIndex == SCXML_TREE_NODE_UNKNOWN || //
				TreeTests->Items->Item[i]->ImageIndex == SCXML_TREE_NODE_STARTED) {

				TreeTests->Items->Item[i]->ImageIndex = SCXML_TREE_NODE_STARTED;

				const AnsiString sScxmlFile = TPath::Combine(FDirW3C, TreeTests->Items->Item[i]->Text);

				usclib_RegisterInterpreterStoppedCallback(g_Interpreter, OnInterpreterStopped, TreeTests->Items->Item[i]);

				Log("Starting " + TreeTests->Items->Item[i]->Text + "...", USCLIB_LOG_INFO);
				if (TreeTests->Items->Item[i]->Data) {
					Log("Test is manual or restricted!", USCLIB_LOG_WARN);
				}

				if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, sScxmlFile.c_str(), false)) {
					TreeTests->Items->Item[i]->ImageIndex = SCXML_TREE_NODE_ERROR;
					TreeTests->Items->Item[i]->SelectedIndex = TreeTests->Items->Item[i]->ImageIndex;
					throw Exception(usclib_GetLastError());
				}

				break;
			}
		}
	}
	catch(Exception * E) {
		Log("ERROR> " + E->Message, USCLIB_LOG_ERROR);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormW3C::BtnStartClick(TObject *Sender) {
	BtnStart->Enabled = false;
	BtnStop->Enabled = true;

	try {
		if (!g_Interpreter) {
			// some test have planned errors, so skip interpreter issue checks,
			// which will terminate execution
			UsclibInterpreterOptions AInterpreterOptions;
			if (USCLIB_SUCCESS != usclib_GetDefaultInterpreterOptions(&AInterpreterOptions))
				throw Exception(usclib_GetLastError());
			AInterpreterOptions.CheckIssues = false;

			if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, &AInterpreterOptions))
				throw Exception(usclib_GetLastError());

			usclib_RegisterLogCallback(g_Interpreter, OnInterpreterLog, NULL);
		}

		InterpreterStartNext();
	}
	catch(Exception * E) {
		Log("ERROR> " + E->Message, USCLIB_LOG_ERROR);
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormW3C::BtnStopClick(TObject *Sender) {
	BtnStart->Enabled = true;
	BtnStop->Enabled = false;

	try {
		if (g_Interpreter) {
			if (USCLIB_SUCCESS != usclib_StopInterpreter(g_Interpreter))
				throw Exception(usclib_GetLastError());
		}
	}
	catch(Exception * E) {
		Log("ERROR> " + E->Message, USCLIB_LOG_ERROR);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormW3C::BtnResetClick(TObject *Sender) {
	for (int i = 0; i < TreeTests->Items->Count; i++) {
		TreeTests->Items->Item[i]->ImageIndex = SCXML_TREE_NODE_UNKNOWN;
		TreeTests->Items->Item[i]->SelectedIndex = TreeTests->Items->Item[i]->ImageIndex;
	}
	RichEdit1->Clear();
	ProgressBar1->Position = 0;
}
//---------------------------------------------------------------------------
