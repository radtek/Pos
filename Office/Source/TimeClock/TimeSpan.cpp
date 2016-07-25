//---------------------------------------------------------------------------
#pragma hdrstop

#include "TimeSpan.h"
#include <SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
TimeSpan::TimeSpan()
{
	Set(0, 0, 0);
}
//---------------------------------------------------------------------------
// use ExtractTime or FromDateTime
//---------------------------------------------------------------------------
TimeSpan::TimeSpan(TDateTime time)
{
	Set(time);
}
//---------------------------------------------------------------------------
TimeSpan::TimeSpan(int hour, int minute, int second)
{
	Set(hour, minute, second);
}
//---------------------------------------------------------------------------
TimeSpan::TimeSpan(const TimeSpan& timeSpan)
{
	Set(timeSpan.hour, timeSpan.minute, timeSpan.second);
}
//---------------------------------------------------------------------------
const TimeSpan& TimeSpan::Empty()
{
	static const TimeSpan emptyTimeSpan = TimeSpan();

	return emptyTimeSpan;
}
//---------------------------------------------------------------------------
bool TimeSpan::IsEmpty() const
{
	return *this == TimeSpan::Empty();
}
//---------------------------------------------------------------------------
TimeSpan TimeSpan::ExtractTime(TDateTime dateTime)
{
	if ((double)dateTime < 0.0)
	{
		throw Exception("Unable to create a -ve time span");
	}
	return TimeSpan(dateTime - int(dateTime));
}
//---------------------------------------------------------------------------
TimeSpan TimeSpan::FromDateTime(TDateTime dateTime)
{
	return TimeSpan(dateTime);
}
//---------------------------------------------------------------------------
void TimeSpan::Set(int hour, int minute, int second)
{
	if (hour < 0 || minute < 0 || second < 0)
	{
		throw Exception("Unable to create a -ve time span");
	}

	this->hour = hour;
	this->minute = minute;
	this->second = second;
}
//---------------------------------------------------------------------------
void TimeSpan::Set(TDateTime dateTime)
{
	if ((double)dateTime < 0.0)
	{
		throw Exception("Unable to create a -ve time span");
	}

	unsigned short hr;
	unsigned short min;
	unsigned short sec;
	unsigned short msec;

	dateTime.DecodeTime(&hr, &min, &sec, &msec);

	Set(hr + (((int)dateTime) * 24), min, sec);
}
//---------------------------------------------------------------------------
bool TimeSpan::operator == (const TimeSpan& rhs) const
{
	return (
		hour == rhs.hour &&
		minute == rhs.minute &&
		second == rhs.second);
}
//---------------------------------------------------------------------------
bool TimeSpan::operator != (const TimeSpan& rhs) const
{
	return !(this->operator == (rhs));
}
//---------------------------------------------------------------------------
bool TimeSpan::operator < (const TimeSpan& rhs) const
{
	return (
		hour < rhs.hour ||
		(hour == rhs.hour && minute < rhs.minute) ||
		(hour == rhs.hour && minute == rhs.minute && second < rhs.second));
}
//---------------------------------------------------------------------------
bool TimeSpan::operator > (const TimeSpan& rhs) const
{
	return ((this->operator == (rhs)) == false && (this->operator < (rhs)) == false);
}
//---------------------------------------------------------------------------
bool TimeSpan::operator <= (const TimeSpan& rhs) const
{
	return ((this->operator < (rhs)) == true || (this->operator == (rhs)) == true);
}
//---------------------------------------------------------------------------
bool TimeSpan::operator >= (const TimeSpan& rhs) const
{
	return ((this->operator > (rhs)) == true || (this->operator == (rhs)) == true);
}
//---------------------------------------------------------------------------
TimeSpan TimeSpan::operator - (const TimeSpan& rhs) const
{
	return TimeSpan(ToDateTime() - rhs.ToDateTime());
}
//---------------------------------------------------------------------------
TimeSpan TimeSpan::operator + (const TimeSpan& rhs) const
{
	return TimeSpan(ToDateTime() + rhs.ToDateTime());
}
//---------------------------------------------------------------------------
TimeSpan& TimeSpan::operator += (const TimeSpan& rhs)
{
	Set(ToDateTime() + rhs.ToDateTime());

	return *this;
}
//---------------------------------------------------------------------------
TimeSpan& TimeSpan::AddDays(int days)
{
	hour += (days * 24);

	return *this;
}
//---------------------------------------------------------------------------
TDateTime TimeSpan::ToDateTime() const
{
	return TDateTime(
		((double)hour / 24.0) +
		((double)minute / 1440) +
		((double)second / 86400));
}
//---------------------------------------------------------------------------
AnsiString TimeSpan::ToString() const
{
	return ToString("0.###");
}
//---------------------------------------------------------------------------
AnsiString TimeSpan::ToString(const AnsiString& format) const
{
	return FormatFloat(format, (double)ToDateTime() * 24.0);
}
//---------------------------------------------------------------------------

