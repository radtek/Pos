//---------------------------------------------------------------------------
#pragma hdrstop

#include "FileBasedPayroll.h"
#include <SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Payroll;
//---------------------------------------------------------------------------
FileBasedPayroll::FileBasedPayroll(const AnsiString& directory)
{
	this->directory = IncludeTrailingBackslash(directory);
}
//---------------------------------------------------------------------------
void FileBasedPayroll::Export()
{
	csv.SaveToFile(directory + GetFileName());
	csv.SaveToFile(directory + "Payroll-" + Now().FormatString("yyyy_mm_dd hh_nn_ss") + ".txt");
}
//---------------------------------------------------------------------------
