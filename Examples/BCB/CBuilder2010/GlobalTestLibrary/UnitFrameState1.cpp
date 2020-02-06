//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFrameState1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameState1 *FrameState1 = NULL;

//---------------------------------------------------------------------------
__fastcall TFrameState1::TFrameState1(TComponent* Owner) : TFrame(Owner) {
}

//---------------------------------------------------------------------------
void TFrameState1::OnGlobalDataReceived(const UnicodeString &sName, const UnicodeString &sData) {
	for (int i = 0; i < this->ControlCount; i++) {
		if (this->Controls[i]->Hint == sName) {
			if (TLabel * ALabel = dynamic_cast<TLabel*>(this->Controls[i])) {
				ALabel->Caption = sName + "=" + sData;
			}
			else if (TMemo * AMemo = dynamic_cast<TMemo*>(this->Controls[i])) {
				AMemo->Text = sName + " =" + sData;
			}
		}
	}
}
//---------------------------------------------------------------------------
