//---------------------------------------------------------------------------
#pragma hdrstop

#include "Worker.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
Worker::Worker(const AnsiString& id, const AnsiString& name)
{
	this->id = id;
	this->name = name;
}
//---------------------------------------------------------------------------
Worker::~Worker()
{
	std::map<Date, WorkDay*>::iterator i;

	for (i=daysWorked.begin(); i!=daysWorked.end(); ++i)
	{
		delete i->second;
	}
}
//---------------------------------------------------------------------------
const AnsiString& Worker::GetId() const
{
	return id;
}
//---------------------------------------------------------------------------
void Worker::AddWorkPeriod(TDateTime start, TDateTime end, TDateTime Break, const AnsiString& department)
{
 	if (end > start)
	{
		WorkDay& workDay = GetWorkDay(start);
  		workDay.AddWorkPeriod(WorkPeriod(start, end, Break, department));
   	}
}
//---------------------------------------------------------------------------
const std::map<Date, WorkDay*>& Worker::GetDaysWorked() const
{
	return daysWorked;
}
//---------------------------------------------------------------------------
AnsiString Worker::ToString() const
{
	return name;
}
//---------------------------------------------------------------------------
WorkDay& Worker::GetWorkDay(Date date)
{
	if (daysWorked.find(date) == daysWorked.end())
	{
		daysWorked[date] = new WorkDay(date);
	}

	return *daysWorked[date];
}
//---------------------------------------------------------------------------
