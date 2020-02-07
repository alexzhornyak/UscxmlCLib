//---------------------------------------------------------------------------

#ifndef UnitTwoTrafficLightsH
#define UnitTwoTrafficLightsH
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
	TLabel *Label1;
	TLabel *Label2;
	TShape *Shape1;
	TShape *ShapeRed2;
	TShape *ShapeYellow2;
	TShape *ShapeGreen2;
	TCheckBox *CheckNightMode2;
	TCheckBox *CheckPause1;
	TCheckBox *CheckPause2;
	void __fastcall CheckNightModeClick(TObject *Sender);
	void __fastcall CheckNightMode2Click(TObject *Sender);
	void __fastcall CheckPause1Click(TObject *Sender);
	void __fastcall CheckPause2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormTrafficLight(TComponent* Owner);
	virtual __fastcall ~TFormTrafficLight(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTrafficLight *FormTrafficLight;
//---------------------------------------------------------------------------
#endif
