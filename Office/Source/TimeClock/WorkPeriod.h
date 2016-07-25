//---------------------------------------------------------------------------
#ifndef WorkPeriodH
#define WorkPeriodH
//---------------------------------------------------------------------------
#include <System.hpp>
#include "TimeSpan.h"
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class WorkPeriod
{
public:

	WorkPeriod(TDateTime periodStart, TDateTime periodEnd, TDateTime BreakDuration, const AnsiString& department);

	const TDateTime& GetStart() const;
	const TDateTime& GetEnd() const;
	const TimeSpan& Duration() const;
    const TDateTime& GetBreak() const;
	const AnsiString& WorkPeriod::GetDepartment() const;

private:

	const TDateTime start;
	const TDateTime end;
	const AnsiString department;
    const TDateTime Break;

	TimeSpan duration;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
