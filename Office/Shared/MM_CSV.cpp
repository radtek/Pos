//---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_CSV.h"
#include <SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace MenuMate
{
//---------------------------------------------------------------------------
namespace CSV
{
//---------------------------------------------------------------------------
AnsiString ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	AnsiString Temp = "";

	int StartPos			= Offset + 1;	// Not Zero based indexing.
	bool Escaped			= false;
	bool InsideQualifier	= false;
	bool Completed			= false;

	for (int i=StartPos; i<=Line.Length() && !Completed; i++)
	{
		if (Escape != NULL && Line[i] == Escape)
		{
			if (Escaped)
			{
				Temp += Escape;
				Escaped = false;
			}
			else
			{
				Escaped = true;
			}
		}
		else if (Qualifier != NULL && Line[i] == Qualifier)	// Check for "
		{
			if (Escaped)
			{
				Temp += Qualifier;
				Escaped = false;
			}
			else
			{
				InsideQualifier = !InsideQualifier;
			}
		}
		else if (Line[i] == Delimiter)	// Check for ,
		{
			if (Escaped)
			{
				Temp += Delimiter;
				Escaped = false;
			}
			else if (InsideQualifier)
			{
				Temp += Delimiter;
			}
			else
			{
				Completed = true;
			}
		}
		else
		{
			if (Escaped)
			{
				Temp += Escape;	// Shouldn't be escape. Assume the Escape char was part of the word.
				Escaped = false;
			}
			Temp += Line[i];
		}
		Offset++;
	}
	return Temp.Trim();
}
//---------------------------------------------------------------------------
AnsiString ReadCSVString(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	return ReadCSVWord(Line, Offset, Delimiter, Qualifier, Escape);
}
//---------------------------------------------------------------------------
WideString ReadCSVWideString(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	return UTF8Decode(ReadCSVWord(Line, Offset, Delimiter, Qualifier, Escape));
}
//---------------------------------------------------------------------------
bool ReadCSVBool(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	AnsiString word = ReadCSVWord(Line, Offset, Delimiter, Qualifier, Escape).UpperCase();
	return word == "T" || word == "TRUE" || word == "1";
}
//---------------------------------------------------------------------------
int ReadCSVInt(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	return StrToInt(ReadCSVWord(Line, Offset, Delimiter, Qualifier, Escape));
}
//---------------------------------------------------------------------------
Currency ReadCSVCurrency(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	return StrToCurr(ReadCSVWord(Line, Offset, Delimiter, Qualifier, Escape));
}
//---------------------------------------------------------------------------
} // namespace CSV
} // namespace MenuMate
//---------------------------------------------------------------------------
using namespace MenuMate::CSV;
//---------------------------------------------------------------------------
CSVLine::operator const AnsiString& () const
{
	return _line;
}
//---------------------------------------------------------------------------
CSVLine& CSVLine::operator << (const AnsiString& str)
{
	addToLine(str);
	return *this;
}
//---------------------------------------------------------------------------
CSVLine& CSVLine::operator << (const WideString& str)
{
	addToLine(UTF8Encode(str));
	return *this;
}
//---------------------------------------------------------------------------
CSVLine& CSVLine::operator << (long integer)
{
	addToLine(IntToStr(integer));
	return *this;
}
//---------------------------------------------------------------------------
CSVLine& CSVLine::operator << (bool boolean)
{
	addToLine(boolean?"True":"False");
	return *this;
}
//---------------------------------------------------------------------------
CSVLine& CSVLine::operator << (Currency currency)
{
	addToLine(FormatFloat("0.####", currency));
	return *this;
}
//---------------------------------------------------------------------------
void CSVLine::addToLine(const AnsiString& str)
{
	if(_line != "")
	{
		_line += ",";
	}
	_line += "\"" + escapeString(str) + "\"";
}
//---------------------------------------------------------------------------
AnsiString CSVLine::escapeString(AnsiString line)
{
	const char qualifier='\"';
	const char escape='\\';
	AnsiString temp;
	for (int i=1; i<=line.Length(); i++)
	{
		if (line[i] == qualifier || line[i] == escape)
		{
			temp += escape;
		}
		temp += line[i];
	}
	return temp;
}
//---------------------------------------------------------------------------
bool CSVLine::IsEmpty() const
{
	return _line.IsEmpty();
}
//---------------------------------------------------------------------------
void CSVLine::Clear()
{
	_line = "";
}
//---------------------------------------------------------------------------

