//---------------------------------------------------------------------------
#ifndef MYOBPayrollH
#define MYOBPayrollH
//---------------------------------------------------------------------------
#include "FileBasedPayroll.h"
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class MYOBPayroll : public FileBasedPayroll
{
public:

	MYOBPayroll(const AnsiString& directory);

	virtual void WorkPeriod(
		const Payroll::Worker& worker,
		const Payroll::WorkDay& workDay,
		const Payroll::WorkPeriod& workPeriod);

protected:

	AnsiString GetFileName() const;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
