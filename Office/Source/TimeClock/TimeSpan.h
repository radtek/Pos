//---------------------------------------------------------------------------
#ifndef TimeSpanH
#define TimeSpanH
//---------------------------------------------------------------------------
#include <System.hpp>
//---------------------------------------------------------------------------
namespace Payroll
{
//---------------------------------------------------------------------------
class TimeSpan
{
public:

	TimeSpan();
	TimeSpan(int hour, int minute, int second);
	TimeSpan(const TimeSpan& timeSpan);

	static TimeSpan ExtractTime(TDateTime dateTime);
	static TimeSpan FromDateTime(TDateTime dateTime);

	static const TimeSpan& Empty() ;
	bool IsEmpty() const;

	void Set(int hour, int minute, int second);
	void Set(TDateTime dateTime);

	bool operator == (const TimeSpan& rhs) const;
	bool operator != (const TimeSpan& rhs) const;
	bool operator < (const TimeSpan& rhs) const;
	bool operator > (const TimeSpan& rhs) const;
	bool operator <= (const TimeSpan& rhs) const;
	bool operator >= (const TimeSpan& rhs) const;
	TimeSpan operator - (const TimeSpan& rhs) const;
	TimeSpan operator + (const TimeSpan& rhs) const;
	TimeSpan& operator += (const TimeSpan& rhs);
	TimeSpan& AddDays(int days);

	TDateTime ToDateTime() const;

	AnsiString ToString() const;
	AnsiString ToString(const AnsiString& format) const;

protected:

	// use ExtractTime or FromDateTime
	TimeSpan(TDateTime time);

	int hour;
	int minute;
	int second;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
