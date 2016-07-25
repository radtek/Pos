//---------------------------------------------------------------------------
#ifndef MM_CSVH
#define MM_CSVH
//---------------------------------------------------------------------------
#include <System.hpp>
//---------------------------------------------------------------------------
namespace MenuMate
{
//---------------------------------------------------------------------------
namespace CSV
{
//---------------------------------------------------------------------------
AnsiString ReadCSVString(AnsiString Line, int& offset, char Delimiter=',', char Qualifier='"', char Escape='\\');
WideString ReadCSVWideString(AnsiString Line, int& offset, char Delimiter=',', char Qualifier='"', char Escape='\\');
bool ReadCSVBool(AnsiString Line, int& offset, char Delimiter=',', char Qualifier='"', char Escape='\\');
int ReadCSVInt(AnsiString Line, int& offset, char Delimiter=',', char Qualifier='"', char Escape='\\');
Currency ReadCSVCurrency(AnsiString Line, int& offset, char Delimiter=',', char Qualifier='"', char Escape='\\');
//---------------------------------------------------------------------------
class CSVLine
{
public:
	operator const AnsiString& () const;
	CSVLine& operator << (const AnsiString& str);
	CSVLine& operator << (const WideString& str);
	CSVLine& operator << (long integer);
	CSVLine& operator << (bool boolean);
	CSVLine& operator << (Currency currency);

	bool IsEmpty() const;
	void Clear();

private:
	AnsiString _line;
	void addToLine(const AnsiString& str);
	AnsiString escapeString(AnsiString line);
};
//---------------------------------------------------------------------------
} // namespace CSV
} // namespace MenuMate
//---------------------------------------------------------------------------
#endif
