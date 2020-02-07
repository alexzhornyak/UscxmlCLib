# KT76C Transponder Simulator

The BendixKing KT 76C transponder - radio transmitter/receiver which operates on radar frequencies

![KT76C_Orig](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/KT76C_Orig.png)

## SCXML Device Logic
![KT76C_Logic](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/KT76C.png)

## Simulator
![KT76C](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/KT76C_App_Example.png)

All buttons have **same handler** which transmit event to state machine with data about button's state (pressed|released)
```
//---------------------------------------------------------------------------
void __fastcall TFormKT76C::SpeedButton1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TSpeedButton * AButton = dynamic_cast<TSpeedButton*>(Sender);
	if (AButton) {
		if (!AButton->Tag) {
			AButton->Tag = 1;
			AButton->Font->Color = clMaroon;

			const AnsiString sName = AButton->Hint;
			usclib_TriggerIntEvent(g_Interpreter, sName.c_str(), 1);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKT76C::SpeedButton1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TSpeedButton * AButton = dynamic_cast<TSpeedButton*>(Sender);
	if (AButton) {
		if (AButton->Tag && !Shift.Contains(ssCtrl)) {
			AButton->Tag = 0;
			AButton->Font->Color = clWindowText;

			const AnsiString sName = AButton->Hint;
			usclib_TriggerIntEvent(g_Interpreter, sName.c_str(), 0);
		}
	}
}
```

For all outputs we also have same handler
```
virtual void __fastcall DoNotify(void) {
	for (int i = 0; i < FormKT76C->ComponentCount; i++) {
		if (TLabel * ALabel = dynamic_cast<TLabel*>(FormKT76C->Components[i])) {
			if (ALabel->Hint == FName) {

				// On-Off controls were marked with 'Tag=1'

				switch(ALabel->Tag) {
				case 1:
					ALabel->Visible = FData.ToIntDef(0);
					break;
				default:
					ALabel->Caption = FData;
					ALabel->Visible = true;
				}
			}
		}
	}
}
```

### So this example is a good demonstration how device logic could be separated from user interface
