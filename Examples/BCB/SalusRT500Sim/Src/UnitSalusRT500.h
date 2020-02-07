//---------------------------------------------------------------------------

#ifndef UnitSalusRT500H
#define UnitSalusRT500H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "cspin.h"
#include <ActnList.hpp>
#include <StdActns.hpp>
#include <Menus.hpp>

#include <memory>
#include "UscxmlCLib.h"
#include "UnitSalusDataModel.h"
//---------------------------------------------------------------------------
class TFormSalusRT500 : public TForm
{
__published:	// IDE-managed Components
	TTrayIcon *TrayIcon1;
	TImage *ImageFrame;
	TPanel *frameOutputs;
	TLabel *labelDayOfWeek;
	TLabel *labelProgramNo;
	TLabel *lcdHour;
	TLabel *lcdMinute;
	TLabel *labelSemicolon;
	TLabel *labelAmPm;
	TLabel *lcdTempInt;
	TLabel *labelDot;
	TLabel *lcdTempFrac;
	TLabel *labelCelcium;
	TImage *ImgDown;
	TImage *ImgUp;
	TImage *ImgHighlight;
	TImage *ImgSelect;
	TImage *ImgSet;
	TImage *ImgReset;
	TImage *ImgSnow;
	TImage *ImgFlame;
	TPanel *Panel1;
	TCheckBox *CheckPower;
	TLabel *Label5;
	TCSpinEdit *SpinTemperature;
	TLabel *labelSet;
	TLabel *labelProg;
	TTimer *TimerFlame;
	TTimer *TimerFreeze;
	TActionList *ActionList1;
	TFileOpen *FileOpenProgram;
	TFileSaveAs *FileSaveProgram;
	TPopupMenu *PopupMenu1;
	TMenuItem *Open1;
	TMenuItem *SaveAs1;
	void __fastcall BtnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall BtnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall CheckPowerClick(TObject *Sender);
	void __fastcall SpinTemperatureChange(TObject *Sender);
	void __fastcall TimerFlameTimer(TObject *Sender);
	void __fastcall TimerFreezeTimer(TObject *Sender);
	void __fastcall FileOpenProgramAccept(TObject *Sender);
	void __fastcall FileSaveProgramAccept(TObject *Sender);
private:	// User declarations
	UsclibInterpreter *FInterpreter;
public:		// User declarations
	__fastcall TFormSalusRT500(TComponent* Owner);
	virtual __fastcall ~TFormSalusRT500(void);

	std::unique_ptr<SalusDataModel> Model;

	void ShowBalloon(const UnicodeString &sMsg, const TBalloonFlags AFlags);

	__property UsclibInterpreter *Interpreter = {read=FInterpreter};
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSalusRT500 *FormSalusRT500;
//---------------------------------------------------------------------------
#endif
