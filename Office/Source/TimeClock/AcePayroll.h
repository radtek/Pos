//---------------------------------------------------------------------------
#ifndef AcePayrollH
#define AcePayrollH
//---------------------------------------------------------------------------
#include "FileBasedPayroll.h"
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class AcePayroll : public FileBasedPayroll
{
public:

	AcePayroll(const AnsiString& directory);

	virtual void WorkerBegin(const Payroll::Worker& worker);

	virtual void WorkPeriod(
		const Payroll::Worker& worker,
		const Payroll::WorkDay& workDay,
		const Payroll::WorkPeriod& workPeriod);

	virtual void WorkerEnd(const Payroll::Worker& worker);

protected:

	virtual AnsiString GetFileName() const;

private:

	Payroll::TimeSpan total;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
