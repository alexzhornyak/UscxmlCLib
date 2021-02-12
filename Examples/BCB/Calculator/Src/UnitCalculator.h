//---------------------------------------------------------------------------

#ifndef UnitCalculatorH
#define UnitCalculatorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormCalculator : public TForm
{
__published:	// IDE-managed Components
	TTrayIcon *TrayIcon1;
	TPanel *Panel1;
	TLabel *LabelDisplay;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TPanel *Panel6;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TButton *Button11;
	TButton *Button12;
	TButton *Button13;
	TButton *Button14;
	TButton *Button15;
	TButton *Button16;
	TButton *Button17;
	void __fastcall ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCalculator(TComponent* Owner);
	virtual __fastcall ~TFormCalculator(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCalculator *FormCalculator;
//---------------------------------------------------------------------------
#endif
