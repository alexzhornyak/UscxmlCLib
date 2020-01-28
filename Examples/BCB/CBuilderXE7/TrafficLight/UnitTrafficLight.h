//---------------------------------------------------------------------------

#ifndef UnitTrafficLightH
#define UnitTrafficLightH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormTrafficLight : public TForm
{
__published:	// IDE-managed Components
	TShape *ShapeRed;
	TShape *ShapeYellow;
	TShape *ShapeGreen;
	TShape *Shape4;
	TCheckBox *CheckNightMode;
	TTrayIcon *TrayIcon1;
	void __fastcall CheckNightModeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormTrafficLight(TComponent* Owner);
	virtual __fastcall ~TFormTrafficLight(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTrafficLight *FormTrafficLight;
//---------------------------------------------------------------------------
#endif
