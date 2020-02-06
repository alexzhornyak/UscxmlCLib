//---------------------------------------------------------------------------

#ifndef UnitFrameState0H
#define UnitFrameState0H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrameState0 : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TTreeView *TreeViewRespond;
	TPanel *Panel1;
	TCheckBox *CheckBox1;
	TTrackBar *TrackBar1;
	TEdit *Edit1;
	TTreeView *TreeView1;
	void __fastcall OnDataChange(TObject *Sender);
	void __fastcall TreeView1Edited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
private:	// User declarations
public:		// User declarations
	__fastcall TFrameState0(TComponent* Owner);

	void __fastcall OnDataRespond(const UnicodeString &sData);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameState0 *FrameState0;
//---------------------------------------------------------------------------
#endif
