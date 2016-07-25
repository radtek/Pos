//---------------------------------------------------------------------------
#pragma hdrstop

#include "DateRangePicker.h"

#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
DateRangePicker::DateRangePicker(
	TDateTimePicker* startDatePicker,
	TDateTimePicker* endDatePicker,
	TComboBox* startTimeHourPicker,
	TComboBox* startTimeMinutePicker,
	TComboBox* endTimeHourPicker,
	TComboBox* endTimeMinutePicker) :

	_startDatePicker(startDatePicker),
	_endDatePicker(endDatePicker),
	_startTimeHourPicker(startTimeHourPicker),
	_startTimeMinutePicker(startTimeMinutePicker),
	_endTimeHourPicker(endTimeHourPicker),
	_endTimeMinutePicker(endTimeMinutePicker),

	OnDateTimeChanged(0),
	OnDateTimeChanging(0),

	_start(0),
	_end(0)
{
	endTimeHourPicker->Items->Clear();
	for (int i=0; i<=23; ++i)
	{
		endTimeHourPicker->Items->Add(FormatFloat("00", i));
   }

	endTimeMinutePicker->Items->Clear();
	for (int i=0; i<=59; ++i)
   {
		endTimeMinutePicker->Items->Add(FormatFloat("00", i));
	}

	startTimeHourPicker->Items->Clear();
	for (int i=0; i<=23; ++i)
	{
		startTimeHourPicker->Items->Add(FormatFloat("00", i));
   }

	startTimeMinutePicker->Items->Clear();
	for (int i=0; i<=59; ++i)
	{
		startTimeMinutePicker->Items->Add(FormatFloat("00", i));
	}

	SelectToday();

	_startDatePicker->OnChange = ControlChange;
	_endDatePicker->OnChange = ControlChange;

	_startTimeHourPicker->OnChange = ControlChange;
	_startTimeMinutePicker->OnChange = ControlChange;
	_endTimeHourPicker->OnChange = ControlChange;
	_endTimeMinutePicker->OnChange = ControlChange;
}
//---------------------------------------------------------------------------
void DateRangePicker::SelectToday()
{
	_startDatePicker->Date = int(Now());
	_endDatePicker->Date = TDateTime(int(IncDay(Now(), 1)));

	SelectDefaultTime();
	InvokeChangeEvent();
}
//---------------------------------------------------------------------------
void DateRangePicker::SelectYesterday()
{
	_startDatePicker->Date = TDateTime(int(IncDay(Now(), -1)));
	_endDatePicker->Date = int(Now());

	SelectDefaultTime();
	InvokeChangeEvent();
}
//---------------------------------------------------------------------------
void DateRangePicker::SetStart(TDateTime start)
{
	SetPicker(start, _startDatePicker, _startTimeHourPicker, _startTimeMinutePicker);
}
//---------------------------------------------------------------------------
void DateRangePicker::SetEnd(TDateTime end)
{
	SetPicker(end, _endDatePicker, _endTimeHourPicker, _endTimeMinutePicker);
}
//---------------------------------------------------------------------------
void DateRangePicker::SetPicker(TDateTime dateTime, TDateTimePicker* datePicker, TComboBox* hourPicker, TComboBox* minPicker)
{
	unsigned short hour, min, sec, msec;

	dateTime.DecodeTime(&hour, &min, &sec, &msec);

	try
	{
		datePicker->OnChange = NULL;
		hourPicker->OnChange = NULL;
		minPicker->OnChange = NULL;

		datePicker->Date = int(dateTime);
		hourPicker->ItemIndex = hour;
		minPicker->ItemIndex = min;
	}
	__finally
	{
		datePicker->OnChange = ControlChange;
		hourPicker->OnChange = ControlChange;
		minPicker->OnChange = ControlChange;
	}
}
//---------------------------------------------------------------------------
TDateTime DateRangePicker::GetStart() const
{
	return GetDateTime(_startDatePicker, _startTimeHourPicker, _startTimeMinutePicker);
}
//---------------------------------------------------------------------------
TDateTime DateRangePicker::GetEnd() const
{
	return GetDateTime(_endDatePicker, _endTimeHourPicker, _endTimeMinutePicker);
}
//---------------------------------------------------------------------------
TDateTime DateRangePicker::GetDateTime(
	TDateTimePicker* datePicker,
	TComboBox* hourPicker,
	TComboBox* minutePicker) const
{
	int startDate = datePicker->Date;
	int startHour = 0;
	int startMinute = 0;

	bool error = false;

	try
	{
		startHour = hourPicker->Text.ToInt();
	}
	catch (EConvertError&)
	{
		hourPicker->Text = "00";
		error = true;
	}

	try
	{
		startMinute = minutePicker->Text.ToInt();
	}
	catch (EConvertError&)
	{
		minutePicker->Text = "00";
		error = true;
	}

	TDateTime result = TDateTime(startDate);
	result += TDateTime(startHour, startMinute, 0, 0);

	if (error)
	{
		//InvokeErrorEvent();
	}

	return result;
}
//---------------------------------------------------------------------------
void DateRangePicker::SelectDefaultTime()
{
	_startTimeHourPicker->ItemIndex = _startTimeHourPicker->Items->IndexOf("05");
	_startTimeMinutePicker->ItemIndex = _startTimeMinutePicker->Items->IndexOf("00");

	_endTimeHourPicker->ItemIndex = _endTimeHourPicker->Items->IndexOf("05");
	_endTimeMinutePicker->ItemIndex = _endTimeMinutePicker->Items->IndexOf("00");
}
//---------------------------------------------------------------------------
void DateRangePicker::InvokeChangeEvent()
{
	if (OnDateTimeChanged != 0)
	{
		TDateTime newStart = GetStart();
		TDateTime newEnd = GetEnd();

		OnDateTimeChanging(_start, newStart, _end, newEnd);

		if (newStart != _start)
		{
			SetStart(newStart);
		}

		if (newEnd != _end)
		{
			SetEnd(newEnd);
		}

		_start = GetStart();
		_end = GetEnd();

		OnDateTimeChanged(_start, _end);
	}
}
//---------------------------------------------------------------------------
void __fastcall DateRangePicker::ControlChange(TObject* sender)
{
	InvokeChangeEvent();
}
//---------------------------------------------------------------------------

