//---------------------------------------------------------------------------
#pragma hdrstop

#include "WorkDay.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
WorkDay::WorkDay(Payroll::Date date)
{
	this->date = date;
}
//---------------------------------------------------------------------------
Payroll::Date WorkDay::GetDateWorked() const
{
	return date;
}
//---------------------------------------------------------------------------
void WorkDay::AddWorkPeriod(WorkPeriod& workPeriod)
{
	workPeriods.push_back(workPeriod);
}
//---------------------------------------------------------------------------
const std::list<WorkPeriod>& WorkDay::GetWorkPeriods() const
{
	return workPeriods;
}
//---------------------------------------------------------------------------
