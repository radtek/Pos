//---------------------------------------------------------------------------
#ifndef WorkDayH
#define WorkDayH
//---------------------------------------------------------------------------
#include "Date.h"
#include "WorkPeriod.h"
#include <list>
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class WorkDay
{
public:

	WorkDay(Payroll::Date date);

	Payroll::Date GetDateWorked() const;
	void AddWorkPeriod(WorkPeriod& workPeriod);
	const std::list<WorkPeriod>& GetWorkPeriods() const;

private:

	Date date;
	std::list<WorkPeriod> workPeriods;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
