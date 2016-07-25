//---------------------------------------------------------------------------
#ifndef WorkerH
#define WorkerH
//---------------------------------------------------------------------------
#include "TimeSpan.h"
#include "WorkDay.h"{
#include <map>

//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class Worker
{
public:

	Worker(const AnsiString& id, const AnsiString& name);
	~Worker();

	const AnsiString& GetId() const;

	void AddWorkPeriod(TDateTime start, TDateTime end, TDateTime Break, const AnsiString& department);

	const std::map<Date, WorkDay*>& GetDaysWorked() const;

	AnsiString ToString() const;

private:

	AnsiString id;
	AnsiString name;

	std::map<Date, WorkDay*> daysWorked;

	WorkDay& GetWorkDay(Date date);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
