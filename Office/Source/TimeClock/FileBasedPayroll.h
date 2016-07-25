//---------------------------------------------------------------------------
#ifndef FileBasedPayrollH
#define FileBasedPayrollH
//---------------------------------------------------------------------------
#include <System.hpp>
#include "TimeClock.h"
#include "Csv.h"
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class FileBasedPayroll : public AbstractPayroll
{
public:

	FileBasedPayroll(const AnsiString& directory);

	virtual void Export();

protected:

	AnsiString directory;
	TCsv csv;
	virtual AnsiString GetFileName() const = 0;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
