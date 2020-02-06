//---------------------------------------------------------------------------

#ifndef UnitLibraryTestH
#define UnitLibraryTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "UscxmlCLib.h"
//---------------------------------------------------------------------------
class TFormGlobalLibraryTest : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit1;
	TSplitter *Splitter1;
	TPanel *PanelInstruments;
	TCheckBox *CheckInitTesting;
	TComboBox *ComboModes;
	TButton *BtnCheckCyrillic;
	TPanel *PanelState2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TMemo *Memo1;
	TCheckBox *CheckPause;
	TLabel *Label4;
	TLabel *Label5;
	void __fastcall CheckInitTestingClick(TObject *Sender);
	void __fastcall ComboModesChange(TObject *Sender);
	void __fastcall BtnCheckCyrillicClick(TObject *Sender);
	void __fastcall CheckPauseClick(TObject *Sender);
private:	// User declarations

	UsclibInterpreter *FInterpreter;

public:		// User declarations
	__fastcall TFormGlobalLibraryTest(TComponent* Owner);
	virtual __fastcall ~TFormGlobalLibraryTest(void);

	void __fastcall Log(const UnicodeString & sMsg, const int ASeverity);

	__property UsclibInterpreter *Interpreter = {read=FInterpreter};
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGlobalLibraryTest *FormGlobalLibraryTest;
//---------------------------------------------------------------------------
#endif
