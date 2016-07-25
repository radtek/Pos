//---------------------------------------------------------------------------
#pragma hdrstop

#include "Date.h"
#include <SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
Date::Date()
{
	Set(0, 0, 0);
}
//---------------------------------------------------------------------------
Date::Date(const TDateTime& dateTime)
{
	unsigned short year, month, day;

	dateTime.DecodeDate(&year, &month, &day);

	Set(year, month, day);
}
//---------------------------------------------------------------------------
void Date::Set(int year, int month, int day)
{
	this->year = year;
	this->month = month;
	this->day = day;
}
//---------------------------------------------------------------------------
bool Date::IsEmpty() const
{
	return *this == Date::Empty();
}
//---------------------------------------------------------------------------
TDateTime Date::AsDateTime() const
{
	return EncodeDate(year, month, day);
}
//---------------------------------------------------------------------------
AnsiString Date::ToString() const
{
	if (IsEmpty())
	{
		return "";
	}
	else
	{
		return AsDateTime().DateString();
	}
}
//---------------------------------------------------------------------------
AnsiString Date::ToString(const AnsiString& format) const
{
	if (IsEmpty())
	{
		return "";
	}
	else
	{
		return AsDateTime().FormatString(format);
	}
}
//---------------------------------------------------------------------------
bool Date::operator == (const Payroll::Date& rhs) const
{
	return
		year == rhs.year &&
		month == rhs.month &&
		day == rhs.day;
}
//---------------------------------------------------------------------------
bool Date::operator < (const Payroll::Date& rhs) const
{
	return
		year < rhs.year ||
		(year == rhs.year && month < rhs.month) ||
		(year == rhs.year && month == rhs.month && day < rhs.day);
}
//---------------------------------------------------------------------------
const Payroll::Date& Date::Empty()
{
	static Date emptyDate = Date();

	return emptyDate;
}
//---------------------------------------------------------------------------

