//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitInterpreterCallbacks.h"

#include <IdSync.hpp>
#include <StrUtils.hpp>

#include "UnitSalusRT500.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

bool g_Closing = false;

//---------------------------------------------------------------------------
class TEnterExitNotify : public TIdNotify {
	const UnicodeString FStateName;
	const bool FEnter;

protected:
	virtual void __fastcall DoNotify(void) {
		if (g_Closing)
			return;

		if (FStateName == "On") {
			if (FEnter) {
				FormSalusRT500->Model->resetToFactorySettings();
			}
		}
		else if (FStateName == "ProgSetup") {
			if (FEnter) {
				FormSalusRT500->Model->resetProgramRecord();
			}
		}
		else if (FStateName == "DisplayFunctional") {
			if (FEnter) {
				FormSalusRT500->Model->resetTemporaryTemperature();
			}
			else {
				FormSalusRT500->Model->setNormalTemperatureMode();
			}
		}
		else if (FStateName == "DisplayTemporary") {
			if (FEnter) {
				FormSalusRT500->Model->setTemporaryTemperatureMode();
			}
		}
		else if (FStateName == "Test") {
			FormSalusRT500->labelDayOfWeek->Caption = "MTUWTHFSASU";
			FormSalusRT500->labelProgramNo->Caption = "H H H H H";
			FormSalusRT500->labelAmPm->Caption = "AM";
			FormSalusRT500->lcdHour->Caption = "HH";
			FormSalusRT500->lcdMinute->Caption = "HH";
			FormSalusRT500->lcdTempInt->Caption = "HH";
			FormSalusRT500->lcdTempFrac->Caption = "H";

			for (int i = 0; i < FormSalusRT500->frameOutputs->ControlCount; i++) {
				TControl * AControl = dynamic_cast<TControl*>(FormSalusRT500->frameOutputs->Controls[i]);
				if (AControl) {
					AControl->Visible = FEnter;
				}
			}
		}
		else if (FStateName == "Operative") {
			FormSalusRT500->labelAmPm->Visible = FEnter;
			FormSalusRT500->labelSemicolon->Visible = FEnter;

			/* Do not Allow users to save & load not in power off mode */
			FormSalusRT500->FileOpenProgram->Enabled = FEnter;
			FormSalusRT500->FileSaveProgram->Enabled = FEnter;
		}
		else if (FStateName == "DisplayNormal") {
			if (FEnter) {
				FormSalusRT500->lcdTempInt->Caption = FormSalusRT500->SpinTemperature->Value;
				FormSalusRT500->lcdTempFrac->Caption = (0);

				const std::pair<int, ProgramItem>programItem = FormSalusRT500->Model->currentProgramItem();
				const int iProgramNo = programItem.first;

				switch(FormSalusRT500->Model->temperatureMode()) {
				case tmdNormal:
					FormSalusRT500->ImgFlame->Visible = FormSalusRT500->SpinTemperature->Value < //
					boost::get<ItemType::Temperature>(programItem.second);
					break;
				case tmdTemporary:
					FormSalusRT500->ImgFlame->Visible = FormSalusRT500->SpinTemperature->Value < //
					FormSalusRT500->Model->temporaryTemperature();
					break;
				case tmdFreeze:
					FormSalusRT500->ImgSnow->Visible = true;
					FormSalusRT500->ImgFlame->Visible = FormSalusRT500->SpinTemperature->Value < //
					FormSalusRT500->Model->freezeTemperature();
					break;
				}

				FormSalusRT500->labelProgramNo->Caption = Strutils::DupeString(" ", iProgramNo) + UnicodeString(iProgramNo + 1);

			}
			else {
				FormSalusRT500->ImgFlame->Visible = false;
				FormSalusRT500->ImgSnow->Visible = false;
			}

			FormSalusRT500->TimerFlame->Tag = 0;
			FormSalusRT500->TimerFlame->Enabled = FormSalusRT500->ImgFlame->Visible;

			FormSalusRT500->TimerFreeze->Tag = 0;
			FormSalusRT500->TimerFreeze->Enabled = FormSalusRT500->ImgSnow->Visible;
		}
		else if (FStateName == "HighLight_On") {
			FormSalusRT500->frameOutputs->Color = TColor(FEnter ? RGB(0, 170, 255) : RGB(135, 177, 160));
		}
		else if (FStateName == "Temperature_On") {
			FormSalusRT500->lcdTempInt->Visible = FEnter;
			FormSalusRT500->lcdTempFrac->Visible = FEnter;
			FormSalusRT500->labelDot->Visible = FEnter;
			FormSalusRT500->labelCelcium->Visible = FEnter;
		}
		else if (FStateName == "TemporaryFlashOn") {
			if (FEnter) {
				FormSalusRT500->lcdTempInt->Caption = FormSalusRT500->Model->getTemporaryTemperatureInt();
				FormSalusRT500->lcdTempFrac->Caption = FormSalusRT500->Model->getTemporaryTemperatureFrac();
			}
		}
		else if (FStateName == "Hour_On") {
			FormSalusRT500->lcdHour->Visible = FEnter;
		}
		else if (FStateName == "Minute_On") {
			FormSalusRT500->lcdMinute->Visible = FEnter;
		}
		else if (FStateName == "Set_On") {
			FormSalusRT500->labelSet->Visible = FEnter;
		}
		else if (FStateName == "Day_On") {
			FormSalusRT500->labelDayOfWeek->Visible = FEnter;
		}
		else if (FStateName == "ProgramNumber_On") {
			FormSalusRT500->labelProgramNo->Visible = FEnter;
		}
	}

public:
	__fastcall TEnterExitNotify(const UnicodeString &sStateName, const bool bEnter) : FStateName(sStateName), FEnter(bEnter) {

	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterEnterExit(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName,
	const bool bEnter, void *AUser) {
	(new TEnterExitNotify(chStateName, bEnter))->Notify();
}

//---------------------------------------------------------------------------
class TEventNotify : public TIdNotify {
	const UnicodeString FEventName;

protected:
	virtual void __fastcall DoNotify(void) {
		if (g_Closing)
			return;

		if (FEventName == "Timer.FreezeMode") {
			FormSalusRT500->Model->flipFreezeTemperatureMode();
			usclib_SetGlobalIntData(FormSalusRT500->Interpreter, "Inp.isFreezeMode", NULL, FormSalusRT500->Model->isFreezeMode() ? 1 : 0,
				0);
		}
		else if (FEventName == "Update.Time") {
			const int iHour = FormSalusRT500->Model->hour();
			FormSalusRT500->lcdHour->Caption = (iHour > 12 ? (iHour - 12) : iHour);
			FormSalusRT500->lcdMinute->Caption = (UnicodeString().sprintf(L"%02d", FormSalusRT500->Model->minute()));
			FormSalusRT500->labelAmPm->Caption = (iHour > 12 ? "  PM" : "AM");
			FormSalusRT500->labelDayOfWeek->Caption = (FormSalusRT500->Model->dayOfWeekToDisplayText(FormSalusRT500->Model->day()));
		}
		else if (FEventName == "Update.Program") {
			FormSalusRT500->labelDayOfWeek->Caption = (FormSalusRT500->Model->getProgramWeekType() == WorkDays ? "MTUWTHF" : "       SASU");

			const int iProgramNo = FormSalusRT500->Model->getProgramNumber();
			const double dTemp = boost::get<ItemType::Temperature>(FormSalusRT500->Model->programRecordToProgramItem());
			const int iHour = boost::get<ItemType::Hour>(FormSalusRT500->Model->programRecordToProgramItem());
			const int iMinute = boost::get<ItemType::Minute>(FormSalusRT500->Model->programRecordToProgramItem());

			FormSalusRT500->lcdHour->Caption = (iHour > 12 ? (iHour - 12) : iHour);
			FormSalusRT500->lcdMinute->Caption = (UnicodeString().sprintf(L"%02d", iMinute));
			FormSalusRT500->labelAmPm->Caption = (iHour > 12 ? "  PM" : "AM");

			FormSalusRT500->lcdTempInt->Caption = (floor(dTemp));
			FormSalusRT500->lcdTempFrac->Caption = (floor((dTemp - floor(dTemp)) * 10.0f));

			FormSalusRT500->labelProgramNo->Caption = (Strutils::DupeString(" ", iProgramNo) + UnicodeString(iProgramNo + 1));
		}
		/* here is technique how to call methods by name */
		else if (void*AMethodCode = FormSalusRT500->Model->MethodAddress(FEventName)) {
			System::TMethod AMethod;
			AMethod.Data = FormSalusRT500->Model.get();
			AMethod.Code = AMethodCode;
			TMachineEvent AMachineEvent = *(TMachineEvent*) & AMethod;
			AMachineEvent();
		}
	}

public:
	__fastcall TEventNotify(const UnicodeString &sEventName) : FEventName(sEventName) {
	}
};

//---------------------------------------------------------------------------
void __stdcall OnInterpreterEvent(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chEventName,
	const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser) {
	(new TEventNotify(chEventName))->Notify();
}
