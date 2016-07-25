//---------------------------------------------------------------------------
#pragma hdrstop

#include "WorkPeriod.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
WorkPeriod::WorkPeriod(TDateTime periodStart, TDateTime periodEnd, TDateTime BreakDuration, const AnsiString& dep) :
	start(periodStart),
	end(periodEnd),
	department(dep),
    Break(BreakDuration)
{
	this->duration = TimeSpan::FromDateTime(end - start - Break);
}
//---------------------------------------------------------------------------
const TDateTime& WorkPeriod::GetStart() const
{
	return start;
}
//---------------------------------------------------------------------------
const TDateTime& WorkPeriod::GetEnd() const
{
	return end;
}
//---------------------------------------------------------------------------
const TimeSpan& WorkPeriod::Duration() const
{
	return duration;
}
//---------------------------------------------------------------------------
const AnsiString& WorkPeriod::GetDepartment() const
{
	return department;
}
//---------------------------------------------------------------------------
const TDateTime& WorkPeriod::GetBreak() const
{
	return Break;
}
