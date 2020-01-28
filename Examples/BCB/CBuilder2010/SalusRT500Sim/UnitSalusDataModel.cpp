//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitSalusDataModel.h"
#include "UnitDefaultSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

SalusDataModel::SalusDataModel() : _hour(12), _minute(0), _day(0), _temperatureMode(tmdNormal), _temporaryTemperature(15),
_freezeTemperature(5) {
	resetToFactorySettings();
}

void SalusDataModel::resetToFactorySettings() {
	resetProgramRecord();

	_di_IXMLDocument ADoc = LoadXMLData(g_DEFAULT_SALUS_SETTINGS);
	if (!ADoc)
		throw Exception("Can not load XML default settings!");

	loadFromXML(ADoc->DocumentElement);
}

void SalusDataModel::loadFromFile(const UnicodeString &sFileName) {
	_di_IXMLDocument ADoc = LoadXMLDocument(sFileName);
	if (!ADoc)
		throw Exception("Can not load [" + sFileName + "]!");
	loadFromXML(ADoc->DocumentElement);
}

void SalusDataModel::saveToFile(const UnicodeString &sFileName) {
	_di_IXMLDocument ADoc = NewXMLDocument();
	ADoc->AddChild("root");
	saveToXML(ADoc->DocumentElement);
	ADoc->SaveToFile(sFileName);
}

std::pair<int/*Number*/, ProgramItem>SalusDataModel::currentProgramItem()const {
	const int i_SATURDAY = 5;
	const ProgramItem *program = _day < i_SATURDAY ? _program[WorkDays] : _program[WeekEnds];
	for (int k = g_MAX_PROGRAM_ITEMS_COUNT - 1; k >= 0; k--) {
		const TDateTime programTime = EncodeTime(boost::get<ItemType::Hour>(program[k]), boost::get<ItemType::Minute>(program[k]), 0, 0);
		const TDateTime currentTime = EncodeTime(_hour, _minute, 0, 0);
		if (currentTime > programTime)
			return std::make_pair(k, program[k]);
		else
			continue;
	}
	return std::make_pair(g_MAX_PROGRAM_ITEMS_COUNT - 1, program[g_MAX_PROGRAM_ITEMS_COUNT - 1]);
}

void SalusDataModel::incrementTemporaryTemperature() {
		if (_temporaryTemperature < 30.0) _temporaryTemperature+= 0.5;
}

void SalusDataModel::decrementTemporaryTemperature() {
	if (_temporaryTemperature < 30.0) _temporaryTemperature-= 0.5;
}

void SalusDataModel::resetTemporaryTemperature() {
	const std::pair<int, ProgramItem>programItem = this->currentProgramItem();
	_temporaryTemperature = boost::get<ItemType::Temperature>(programItem.second);
}

void SalusDataModel::incrementHour() {
	if (_hour < 23) {
		_hour++;
	}
}

void SalusDataModel::decrementHour() {
	if (_hour > 0) {
		_hour--;
	}
}

void SalusDataModel::incrementMinute() {
	if (_minute < 59) {
		_minute++;
	}
}

void SalusDataModel::decrementMinute() {
	if (_minute > 0) {
		_minute--;
	}
}

void SalusDataModel::incrementDay() {
	if (_day < 6) {
		_day++;
	}
}

void SalusDataModel::decrementDay() {
	if (_day > 0) {
		_day--;
	}
}

void SalusDataModel::incrementProgramHour() {
	int iMax = 23;
	if (_programRecord.second < g_MAX_PROGRAM_ITEMS_COUNT - 1) {
		// get Hours in the next element
		iMax = boost::get<ItemType::Hour>(_program[_programRecord.first][_programRecord.second + 1]) - 1;
	}

	if (boost::get<ItemType::Hour>(_program[_programRecord.first][_programRecord.second]) < iMax) {
		boost::get<ItemType::Hour>(_program[_programRecord.first][_programRecord.second])++;
	}
}

void SalusDataModel::decrementProgramHour() {
	int iMin = 0;
	if (_programRecord.second > 0) {
		// get Hours in the previous element
		iMin = boost::get<ItemType::Hour>(_program[_programRecord.first][_programRecord.second - 1]) + 1;
	}

	if (boost::get<ItemType::Hour>(_program[_programRecord.first][_programRecord.second]) > iMin) {
		boost::get<ItemType::Hour>(_program[_programRecord.first][_programRecord.second])--;
	}
}

void SalusDataModel::incrementProgramMinute() {
	if (boost::get<ItemType::Minute>(_program[_programRecord.first][_programRecord.second]) < 59) {
		boost::get<ItemType::Minute>(_program[_programRecord.first][_programRecord.second])++;
	}
}

void SalusDataModel::decrementProgramMinute() {
	if (boost::get<ItemType::Minute>(_program[_programRecord.first][_programRecord.second]) > 0) {
		boost::get<ItemType::Minute>(_program[_programRecord.first][_programRecord.second])--;
	}
}

void SalusDataModel::incrementProgramTemperature() {
	if (boost::get<ItemType::Temperature>(_program[_programRecord.first][_programRecord.second]) < 30) {
		boost::get<ItemType::Temperature>(_program[_programRecord.first][_programRecord.second]) += 0.5;
	}
}

void SalusDataModel::decrementProgramTemperature() {
	if (boost::get<ItemType::Temperature>(_program[_programRecord.first][_programRecord.second]) > 0) {
		boost::get<ItemType::Temperature>(_program[_programRecord.first][_programRecord.second]) -= 0.5;
	}
}

void SalusDataModel::incrementProgramNumber() {
	if (_programRecord.second < g_MAX_PROGRAM_ITEMS_COUNT - 1) {
		_programRecord.second++;
	}
}

void SalusDataModel::decrementProgramNumber() {
	if (_programRecord.second > 0) {
		_programRecord.second--;
	}
}

void SalusDataModel::loadFromXML(_di_IXMLNode program) {
	if (!program)
		throw Exception("Root node is empty!");

	_hour = program->ChildValues["Hour"];
	_minute = program->ChildValues["Minute"];
	_day = program->ChildValues["Day"];

	_di_IXMLNode AWorkDays = program->ChildNodes->FindNode("WorkDays");
	if (!AWorkDays)
		throw Exception("'WorkDays' node is not found!");

	if (AWorkDays->ChildNodes->Count != g_MAX_PROGRAM_ITEMS_COUNT)
		throw Exception("Node 'WorkDays' require " + UnicodeString(g_MAX_PROGRAM_ITEMS_COUNT) + "Items! (Comments are not permitted!)");

	for (int i = 0; i < AWorkDays->ChildNodes->Count; i++) {
		boost::get<ItemType::Hour>(_program[WorkDays][i]) = AWorkDays->ChildNodes->Nodes[i]->ChildValues["Hour"];
		boost::get<ItemType::Minute>(_program[WorkDays][i]) = AWorkDays->ChildNodes->Nodes[i]->ChildValues["Minute"];
		boost::get<ItemType::Temperature>(_program[WorkDays][i]) = AWorkDays->ChildNodes->Nodes[i]->ChildValues["Temperature"];
	}

	_di_IXMLNode AWeekEnds = program->ChildNodes->FindNode("WeekEnds");
	if (!AWeekEnds)
		throw Exception("'WeekEnds' node is not found!");

	if (AWeekEnds->ChildNodes->Count != g_MAX_PROGRAM_ITEMS_COUNT)
		throw Exception("Node 'WeekEnds' require " + UnicodeString(g_MAX_PROGRAM_ITEMS_COUNT) + "Items! (Comments are not permitted!)");

	for (int i = 0; i < AWeekEnds->ChildNodes->Count; i++) {
		boost::get<ItemType::Hour>(_program[WeekEnds][i]) = AWeekEnds->ChildNodes->Nodes[i]->ChildValues["Hour"];
		boost::get<ItemType::Minute>(_program[WeekEnds][i]) = AWeekEnds->ChildNodes->Nodes[i]->ChildValues["Minute"];
		boost::get<ItemType::Temperature>(_program[WeekEnds][i]) = AWeekEnds->ChildNodes->Nodes[i]->ChildValues["Temperature"];
	}
}

void SalusDataModel::saveToXML(_di_IXMLNode program) {
	if (!program)
		throw Exception("Root node is empty!");

	program->ChildNodes->Clear();

	program->ChildValues["Hour"] = _hour;
	program->ChildValues["Minute"] = _minute;
	program->ChildValues["Day"] = _day;

	_di_IXMLNode AWorkDays = program->AddChild("WorkDays");
	for (int k = 0; k < g_MAX_PROGRAM_ITEMS_COUNT; k++) {
		_di_IXMLNode AItem = AWorkDays->AddChild("Item");
		AItem->ChildValues["Hour"] = boost::get<ItemType::Hour>(_program[WorkDays][k]);
		AItem->ChildValues["Minute"] = boost::get<ItemType::Minute>(_program[WorkDays][k]);
		AItem->ChildValues["Temperature"] = boost::get<ItemType::Temperature>(_program[WorkDays][k]);
	}

	_di_IXMLNode AWeekEnds = program->AddChild("WeekEnds");
	for (int k = 0; k < g_MAX_PROGRAM_ITEMS_COUNT; k++) {
		_di_IXMLNode AItem = AWeekEnds->AddChild("Item");
		AItem->ChildValues["Hour"] = boost::get<ItemType::Hour>(_program[WeekEnds][k]);
		AItem->ChildValues["Minute"] = boost::get<ItemType::Minute>(_program[WeekEnds][k]);
		AItem->ChildValues["Temperature"] = boost::get<ItemType::Temperature>(_program[WeekEnds][k]);
	}
}

UnicodeString SalusDataModel::dayOfWeekToDisplayText(const int day)
{
    // MTUWTHFSASU
    switch (day) {
    case 0: return "M";
    case 1: return " TU";
    case 2: return "   W";
    case 3: return "    TH";
    case 4: return "      F";
    case 5: return "       SA";
    case 6: return "         SU";
    }
	throw Exception("Day is out of range!");
}
