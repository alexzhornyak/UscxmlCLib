//---------------------------------------------------------------------------

#ifndef UnitHelloWorldH
#define UnitHelloWorldH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormHelloWorld : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TButton *BtnStart;
	void __fastcall BtnStartClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormHelloWorld(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHelloWorld *FormHelloWorld;
//---------------------------------------------------------------------------
#endif
