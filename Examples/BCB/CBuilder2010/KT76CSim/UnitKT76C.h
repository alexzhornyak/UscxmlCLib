//---------------------------------------------------------------------------

#ifndef UnitKT76CH
#define UnitKT76CH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormKT76C : public TForm
{
__published:	// IDE-managed Components
	TTrayIcon *TrayIcon1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *BtnSupply;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TSpeedButton *SpeedButton7;
	TSpeedButton *SpeedButton8;
	TSpeedButton *SpeedButton9;
	TSpeedButton *SpeedButton10;
	TSpeedButton *SpeedButton11;
	TSpeedButton *SpeedButton12;
	TTrackBar *TrackBar1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TPanel *Panel1;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	void __fastcall BtnSupplyClick(TObject *Sender);
	void __fastcall TrackBar1Change(TObject *Sender);
	void __fastcall SpeedButton1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
	__fastcall TFormKT76C(TComponent* Owner);
	virtual __fastcall ~TFormKT76C(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKT76C *FormKT76C;
//---------------------------------------------------------------------------
#endif
