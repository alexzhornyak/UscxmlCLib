//---------------------------------------------------------------------------

#ifndef UnitTesterW3CH
#define UnitTesterW3CH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TFormW3C : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *ProgressBar1;
	TPanel *Panel1;
	TButton *BtnStart;
	TButton *BtnStop;
	TSplitter *Splitter1;
	TRichEdit *RichEdit1;
	TTreeView *TreeTests;
	TImageList *ImageList1;
	TButton *BtnReset;
	void __fastcall BtnStartClick(TObject *Sender);
	void __fastcall BtnStopClick(TObject *Sender);
	void __fastcall BtnResetClick(TObject *Sender);
private:	// User declarations
	UnicodeString FDirW3C;
	TDateTime FStarted;
protected:

	void __fastcall ProcessedNode(TTreeNode *ANode, const bool APass);

	friend class TStoppedNotify;

public:		// User declarations
	__fastcall TFormW3C(TComponent* Owner);
	virtual __fastcall ~TFormW3C(void);

	void __fastcall Log(const UnicodeString &sMsg, const int ASeverity);

	void __fastcall InterpreterStartNext();

	inline TDateTime __fastcall Elapsed() { return Now() - FStarted; }
};
//---------------------------------------------------------------------------
extern PACKAGE TFormW3C *FormW3C;
//---------------------------------------------------------------------------
#endif
