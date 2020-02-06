//---------------------------------------------------------------------------

#ifndef UnitFrameState1H
#define UnitFrameState1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrameState1 : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TMemo *Memo1;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TMemo *MemoIOTimerSignal;
private:	// User declarations
public:		// User declarations
	__fastcall TFrameState1(TComponent* Owner);

	void OnGlobalDataReceived(const UnicodeString &sName, const UnicodeString &sData);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameState1 *FrameState1;
//---------------------------------------------------------------------------
#endif
