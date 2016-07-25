//---------------------------------------------------------------------------
#ifndef DateH
#define DateH
//---------------------------------------------------------------------------
#include <System.hpp>
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class Date
{
public:

	Date();
	Date(const TDateTime& dateTime);

	bool IsEmpty() const;

	TDateTime AsDateTime() const;

	AnsiString ToString() const;
	AnsiString ToString(const AnsiString& format) const;

	static const Date& Empty();

	bool operator == (const Date& rhs) const;
	bool operator < (const Date& rhs) const;

private:

	void Set(int year, int month, int day);

	int year;
	int month;
	int day;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
