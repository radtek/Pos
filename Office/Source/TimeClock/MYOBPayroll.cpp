//---------------------------------------------------------------------------
#pragma hdrstop

#include "MYOBPayroll.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
MYOBPayroll::MYOBPayroll(const AnsiString& directory) : FileBasedPayroll(directory)
{
}
//---------------------------------------------------------------------------
void MYOBPayroll::WorkPeriod(
	const Payroll::Worker& worker,
	const Payroll::WorkDay& workDay,
	const Payroll::WorkPeriod& workPeriod)
{
	csv.Add(
		workDay.GetDateWorked().ToString() + "," +
		worker.GetId() + "," +
		"ORD," +
		workPeriod.GetDepartment() +
		",," +
		workPeriod.Duration().ToString());
}
//---------------------------------------------------------------------------
AnsiString MYOBPayroll::GetFileName() const
{
	return "myobpay.txt";
}
//---------------------------------------------------------------------------
