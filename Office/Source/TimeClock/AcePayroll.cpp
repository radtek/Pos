//---------------------------------------------------------------------------
#pragma hdrstop

#include "AcePayroll.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
AcePayroll::AcePayroll(const AnsiString& directory) : FileBasedPayroll(directory)
{
}
//---------------------------------------------------------------------------
void AcePayroll::WorkerBegin(const Payroll::Worker& worker)
{
	total = Payroll::TimeSpan::Empty();
}
//---------------------------------------------------------------------------
void AcePayroll::WorkPeriod(
	const Payroll::Worker& worker,
	const Payroll::WorkDay& workDay,
	const Payroll::WorkPeriod& workPeriod)
{
	total += workPeriod.Duration();
}
//---------------------------------------------------------------------------
void AcePayroll::WorkerEnd(const Payroll::Worker& worker)
{
	if (!total.IsEmpty())
	{
		csv.Add(worker.GetId() + "," + total.ToString() + ",,,,,");
	}
}
//---------------------------------------------------------------------------
AnsiString AcePayroll::GetFileName() const
{
	return "IMPORT.TXT";
}
//---------------------------------------------------------------------------
