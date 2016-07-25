//---------------------------------------------------------------------------
#ifndef TimeClockH
#define TimeClockH
//---------------------------------------------------------------------------
#include "Worker.h"
#include <set>
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class AbstractPayroll
{
public:

	virtual ~AbstractPayroll() {}

	virtual void Initialise() {};

	virtual void WorkerBegin(const Worker& worker) {};

	virtual void DayBegin(const Worker& worker, const WorkDay& workDay) {};

	virtual void WorkPeriod(const Worker& worker, const WorkDay& workDay,
		const WorkPeriod& workPeriod) {};

	virtual void DayEnd(const Worker& worker, const WorkDay& workDay) {};

	virtual void WorkerEnd(const Worker& worker) {};

	virtual void Complete() {};

	virtual void Export() = 0;

};
//---------------------------------------------------------------------------
class UpdateExported
{
public:

	virtual void Update(int key) = 0;
};
//---------------------------------------------------------------------------
class TimeClockExporter
{
public:

	virtual ~TimeClockExporter();

	// Will return the worker object with this ID,
	// creating one if necessary.
	Worker& GetWorker(const AnsiString& id, const AnsiString& name);

	void Parse(AbstractPayroll& output) const;

	void AddKey(int key);
	void Close(UpdateExported* updateExported);

private:

	std::map<AnsiString, Worker*> workers;
	std::set<int> keys;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
