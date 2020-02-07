//---------------------------------------------------------------------------

#ifndef UnitSalusDataModelH
#define UnitSalusDataModelH
//---------------------------------------------------------------------------

#include <boost/tuple/tuple.hpp>

#include <XMLDoc.hpp>
#include <math.h>

static const int g_MAX_PROGRAM_ITEMS_COUNT = 5;

typedef boost::tuple<int/*Hour*/,int/*Minute*/,double/*Temperature*/> ProgramItem;
typedef enum {Hour,Minute,Temperature} ItemType;

typedef enum {WorkDays, WeekEnds} WeekType;
static const int g_MAX_WEEK_TYPE_COUNT = 2;

typedef enum {tmdNormal, tmdTemporary, tmdFreeze} TemperatureMode;

typedef void (__closure *TMachineEvent)();

class SalusDataModel: public TObject
{
// this methods are available to call by name
__published:
	void incrementTemporaryTemperature();
	void decrementTemporaryTemperature();

	void incrementHour();
	void decrementHour();

	void incrementMinute();
	void decrementMinute();

	void incrementDay();
	void decrementDay();

	/* Program Setup */

	// there can be 5/2 mode or 7 mode
	// in this program we are simulating 5/2 mode, so just simply invert
	inline void incrementProgramDay() { _programRecord.first = _programRecord.first == WorkDays ? WeekEnds : WorkDays; }
	inline void decrementProgramDay() { incrementProgramDay(); }

	void incrementProgramHour();
	void decrementProgramHour();

	void incrementProgramMinute();
	void decrementProgramMinute();

	void incrementProgramTemperature();
	void decrementProgramTemperature();

	void incrementProgramNumber();
	void decrementProgramNumber();
public:

	SalusDataModel();

	/* Program Record depending on current day and time settings */
	std::pair<int/*Number*/,ProgramItem> currentProgramItem() const;

    /* Temporary Temperature Setup */
	inline int getTemporaryTemperatureInt() { return floor(_temporaryTemperature); }
    inline int getTemporaryTemperatureFrac() { return (_temporaryTemperature - getTemporaryTemperatureInt())*10.0f; }

	inline const TemperatureMode temperatureMode() const { return _temperatureMode; }
    inline const double temporaryTemperature() const { return _temporaryTemperature; }
    inline const double freezeTemperature() const { return _freezeTemperature; }
    inline const bool isFreezeMode() const { return _temperatureMode==tmdFreeze; }

    void resetTemporaryTemperature();
    inline void setNormalTemperatureMode() { _temperatureMode=tmdNormal; }
    inline void setTemporaryTemperatureMode() { _temperatureMode=tmdTemporary; }
    inline void flipFreezeTemperatureMode() { _temperatureMode=_temperatureMode==tmdFreeze ? tmdNormal : tmdFreeze; }

    /* Day and Time Setup */
    inline const int &hour() const { return _hour; }
    inline const int &minute() const { return _minute; }
    inline const int &day() const { return _day; }

	inline void resetProgramRecord() { _programRecord = std::make_pair(WorkDays,0); }
    inline const WeekType getProgramWeekType() const { return _programRecord.first; }
    inline const int getProgramNumber() const { return _programRecord.second; }
    inline const ProgramItem &programRecordToProgramItem() const {
        return _program[_programRecord.first][_programRecord.second]; }

	static UnicodeString dayOfWeekToDisplayText(const int day);

    /* HARD RESET */
    void resetToFactorySettings();

    void loadFromFile(const UnicodeString &sFileName);
	void saveToFile(const UnicodeString &sFileName);

private:

    ProgramItem _program[g_MAX_WEEK_TYPE_COUNT][g_MAX_PROGRAM_ITEMS_COUNT];

	int _hour; // 0...23
	int _minute; // 0...59
	int _day; // Day of week: 0 ... 6

	TemperatureMode _temperatureMode;
	double _temporaryTemperature;
	const double _freezeTemperature;

	std::pair<WeekType,int/*ProgramNumber*/> _programRecord;

    /* Serialization */
	void loadFromXML(_di_IXMLNode program);

	void saveToXML(_di_IXMLNode program);

};

#endif
