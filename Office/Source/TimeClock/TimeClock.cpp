//---------------------------------------------------------------------------
#pragma hdrstop

#include "TimeClock.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
TimeClockExporter::~TimeClockExporter()
{
	std::map<AnsiString, Worker*>::iterator i;

	for (i=workers.begin(); i!=workers.end(); ++i)
	{
		delete i->second;
	}
}
//---------------------------------------------------------------------------
Worker& TimeClockExporter::GetWorker(const AnsiString& id, const AnsiString& name)
{
	if (workers.find(id) == workers.end())
	{
		workers[id] = new Worker(id, name);
	}

	return *workers[id];
}
//---------------------------------------------------------------------------
void TimeClockExporter::Parse(AbstractPayroll& output) const
{
	output.Initialise();

	std::map<AnsiString, Worker*>::const_iterator i;

	for (i=workers.begin(); i!=workers.end(); ++i)
	{
		const Worker& worker = *(i->second);

		output.WorkerBegin(worker);

		std::map<Date, WorkDay*>::const_iterator j;

		for (j=worker.GetDaysWorked().begin(); j!=worker.GetDaysWorked().end(); ++j)
		{
			const WorkDay& workDay = *(j->second);

			output.DayBegin(worker, workDay);

			std::list<WorkPeriod>::const_iterator k;

			for (k=workDay.GetWorkPeriods().begin(); k!=workDay.GetWorkPeriods().end(); ++k)
			{
				output.WorkPeriod(worker, workDay, *k);
			}

			output.DayEnd(worker, workDay);
		}

		output.WorkerEnd(worker);
	}

	output.Complete();
}
//---------------------------------------------------------------------------
void TimeClockExporter::AddKey(int key)
{
	keys.insert(key);
}
//---------------------------------------------------------------------------
void TimeClockExporter::Close(UpdateExported* updateExported)
{
	if (updateExported != 0)
	{
		std::set<int>::iterator i;

		for (i=keys.begin(); i!=keys.end(); ++i)
		{
			updateExported->Update(*i);
		}
	}
}
//---------------------------------------------------------------------------

