//---------------------------------------------------------------------------
#ifndef DateRangePickerH
#define DateRangePickerH
//---------------------------------------------------------------------------
#include <system.hpp>
//---------------------------------------------------------------------------
namespace Comctrls { class TDateTimePicker; }
namespace Stdctrls { class TComboBox; }
//---------------------------------------------------------------------------
typedef void __fastcall(__closure *TDateTimeChanging)(
	TDateTime OldStart, TDateTime& NewStart,
	TDateTime OldEnd, TDateTime& NewEnd);
typedef void __fastcall(__closure *TDateTimeChanged)(TDateTime NewStart, TDateTime NewEnd);
//---------------------------------------------------------------------------
class DateRangePicker
{
public:
	virtual ~DateRangePicker() {}

	DateRangePicker(
		Comctrls::TDateTimePicker* startDatePicker,
		Comctrls::TDateTimePicker* endDatePicker,
		Stdctrls::TComboBox* startTimeHourPicker,
		Stdctrls::TComboBox* startTimeMinutePicker,
		Stdctrls::TComboBox* endTimeHourPicker,
		Stdctrls::TComboBox* endTimeMinutePicker);

	void SelectToday();
	void SelectYesterday();

	TDateTime GetStart() const;
	TDateTime GetEnd() const;

	TDateTimeChanged OnDateTimeChanged;
	TDateTimeChanging OnDateTimeChanging;

protected:
		Comctrls::TDateTimePicker* _startDatePicker;
		Comctrls::TDateTimePicker* _endDatePicker;
		Stdctrls::TComboBox* _startTimeHourPicker;
		Stdctrls::TComboBox* _startTimeMinutePicker;
		Stdctrls::TComboBox* _endTimeHourPicker;
		Stdctrls::TComboBox* _endTimeMinutePicker;

		void SelectDefaultTime();
		void InvokeChangeEvent();

		void SetStart(TDateTime start);
		void SetEnd(TDateTime end);
		void SetPicker(TDateTime dateTime,
			Comctrls::TDateTimePicker* datePicker,
			Stdctrls::TComboBox* hourPicker,
			Stdctrls::TComboBox* minPicker);

		TDateTime _start;
		TDateTime _end;

		TDateTime GetDateTime(
			Comctrls::TDateTimePicker* datePicker,
			Stdctrls::TComboBox* hourPicker,
			Stdctrls::TComboBox* minutePicker) const;

	void __fastcall ControlChange(TObject* sender);
};
//---------------------------------------------------------------------------
#endif
