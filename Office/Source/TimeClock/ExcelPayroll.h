//---------------------------------------------------------------------------
#ifndef ExcelPayrollH
#define ExcelPayrollH
//---------------------------------------------------------------------------
#include "TimeClock.h"
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class ExcelPayroll : public Payroll::AbstractPayroll
{
public:

	ExcelPayroll();
	~ExcelPayroll();

	virtual void Export();

	virtual void WorkPeriod(
		const Payroll::Worker& worker,
		const Payroll::WorkDay& workDay,
		const Payroll::WorkPeriod& workPeriod);

private:

	class ExcelTimeClockData* const data;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
