//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitHelloWorld.h"

#include <IdSync.hpp>

#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormHelloWorld *FormHelloWorld = NULL;

UsclibInterpreter *g_Interpreter = NULL;

//---------------------------------------------------------------------------
class TLogNotify : public TIdNotify {
	const UnicodeString FMessage;

protected:
	virtual void __fastcall DoNotify(void) {
		FormHelloWorld->Memo1->Lines->Add(FMessage);
	}

public:
	__fastcall TLogNotify(const UnicodeString &sMsg) : FMessage(sMsg) {
	}
};

void __stdcall OnInterpreterLogCallback(const UsclibInterpreter *AInterpreter, const int nSeverity, const char *chMessage, void *AUser) {
	(new TLogNotify(chMessage))->Notify();
}

//---------------------------------------------------------------------------
static const char *g_chScxml = //
"<scxml name=\"Scxml\" version=\"1.0\" xmlns=\"http://www.w3.org/2005/07/scxml\">" //
"	<final id=\"Final\">" //
"		<onentry>" //
"			<log expr=\"Hello, world!\" label=\"INFO\"/>" //
"		</onentry>" //
"	</final>" //
"</scxml>";

//---------------------------------------------------------------------------
//--------------------------- TFormHelloWorld -------------------------------
//---------------------------------------------------------------------------
__fastcall TFormHelloWorld::TFormHelloWorld(TComponent* Owner) : TForm(Owner) {

}

//---------------------------------------------------------------------------
__fastcall TFormHelloWorld::~TFormHelloWorld(void) {
	usclib_GlobalCleanup();
}

//---------------------------------------------------------------------------
void __fastcall TFormHelloWorld::BtnStartClick(TObject *Sender) {
	try {
		if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, 0))
			throw Exception(usclib_GetLastError());

		usclib_RegisterLogCallback(g_Interpreter, OnInterpreterLogCallback, NULL);

		if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, g_chScxml, true))
			throw Exception(usclib_GetLastError());

		if (USCLIB_SUCCESS != usclib_CloseInterpreter(g_Interpreter))
			throw Exception(usclib_GetLastError());
	}
	catch(Exception * E) {
		Memo1->Lines->Add("ERROR> " + E->Message);
	}
}
//---------------------------------------------------------------------------
