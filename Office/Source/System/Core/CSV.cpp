//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CSV.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TCsv::TCsv()
{
	fStrings 	= new TStringList;
	Delimiter 	= ',';
	Qualifier 	= NULL;
	EscapeChar	= '\\';
}
//---------------------------------------------------------------------------
TCsv::~TCsv()
{
	delete fStrings;
}
//---------------------------------------------------------------------------
void TCsv::LoadFromFile(const AnsiString FileName)
{
	fStrings->LoadFromFile(FileName);
}
//---------------------------------------------------------------------------
void TCsv::SaveToFile(const AnsiString FileName)
{
	fStrings->SaveToFile(FileName);
}
//---------------------------------------------------------------------------
int TCsv::GetRowCount()
{
	return fStrings->Count;
}
//---------------------------------------------------------------------------
int TCsv::GetColCount(int ARow)
{
	AnsiString Row = fStrings->Strings[ARow];
	int Count = 0;

	for (int i=0; i<Row.Length(); i++)
	{
		// Skip any white space
		while (Row.c_str()[i] == ' ' && i<Row.Length())
		{
			i++;
		}
		bool Escaped = false;
		if (Qualifier && Row.c_str()[i] == Qualifier)
		{
			// Move though to the end of the qualified word.
			i++;
			while ((Row.c_str()[i] != Qualifier || Escaped) && i<Row.Length())
			{
				if (Row.c_str()[i] == EscapeChar && !Escaped)
				{
					Escaped = true;
				}
				else
				{
					Escaped = false;
				}
				i++;
			}
			i++;	// Skip over the qualifier
		}
		// Skip any white space
		while (Row.c_str()[i] == ' ' && i<Row.Length())
		{
			i++;
		}
		// Keep looking for delimiter
		Escaped = false;
		while ((Row.c_str()[i] != Delimiter || Escaped) && i<Row.Length())
		{
			if (Row.c_str()[i] == EscapeChar && !Escaped)
			{
				Escaped = true;
			}
			else
			{
				Escaped = false;
			}
			i++;
		}
		i++;	// Skip over the delimiter

		Count++;
	}
	return Count;
/*	bool LastCharEscape = false;
	int DelimiterCount = 0;
	for (int i=0; i<Row.Length(); i++)
	{
		if (Row.c_str()[i] == Delimiter && !LastCharEscape)
		{
			DelimiterCount++;
		}
		else if (Row.c_str()[i] == EscapeChar)
		{
			// Already Escaped, therefore it's a legit char.
			if (LastCharEscape)
			{
				LastCharEscape = false;
			}
			// Just an escape...
			else
			{
				LastCharEscape = true;
			}
		}
		else
		{
			LastCharEscape = false;
		}
	}
	return DelimiterCount + 1;*/
}
//---------------------------------------------------------------------------
AnsiString TCsv::GetCells(int ACol, int ARow)
{
	return ColumnText(fStrings->Strings[ARow], ACol);
}
//---------------------------------------------------------------------------
AnsiString TCsv::GetLine(int ARow)
{
	return fStrings->Strings[ARow];
}
//---------------------------------------------------------------------------
void TCsv::SetCells(int ACol, int ARow, const AnsiString Value)
{
	AnsiString Row = fStrings->Strings[ARow];

	bool LastCharEscape = false;
	int DelimiterCount = 0;
	int StartPos = 0, EndPos = 0;
	for (int i=0; i<Row.Length(); i++)
	{
		// Found a delimiter. See if it's the start or end of the word we are looking for.
		if (Row.c_str()[i] == Delimiter && !LastCharEscape)
		{
			if (DelimiterCount == ACol-1)
			{
				StartPos = i + 1;
			}
			else if (DelimiterCount == ACol)
			{
				EndPos = i + 1;
				break;
			}
			DelimiterCount++;
		}
		else if (Row.c_str()[i] == EscapeChar)
		{
			// Already Escaped, therefore it's a legit char.
			if (LastCharEscape)
			{
				LastCharEscape = false;
			}
			// Just an escape...
			else
			{
				LastCharEscape = true;
			}
		}
		else
		{
			LastCharEscape = false;
		}
	}
	if (EndPos == 0) EndPos = Row.Length();
	// Escape any delimiter chars in the new value.
	AnsiString EscapedValue = "";
	for (int i=0; i<Value.Length(); i++)
	{
		if (Value.c_str()[i] == Delimiter)
		{
			EscapedValue += EscapeChar;
		}
		EscapedValue += Value.c_str()[i];
	}
	fStrings->Strings[ARow] = Row.SubString(1, StartPos) + EscapedValue + Row.SubString(EndPos, Row.Length() - EndPos + 1);
}
//---------------------------------------------------------------------------
int TCsv::Add(const AnsiString Value)
{
	return fStrings->Add(Value);
}
//---------------------------------------------------------------------------
void TCsv::DeleteRow(int ARow)
{
	fStrings->Delete(ARow);
}
//---------------------------------------------------------------------------
void TCsv::Clear()
{
	fStrings->Clear();
}
//---------------------------------------------------------------------------
AnsiString TCsv::ColumnText(AnsiString Row, int ACol)
{
	int Count = 0;
	int i=0;

	// Skip first 'ACol' words
	while (Count < ACol && i<Row.Length())
	{
		// Skip any white space
		while (Row.c_str()[i] == ' ' && i<Row.Length())
		{
			i++;
		}
		bool Escaped = false;
		if (Qualifier && Row.c_str()[i] == Qualifier)
		{
			// Move though to the end of the qualified word.
			i++;
			while ((Row.c_str()[i] != Qualifier || Escaped) && i<Row.Length())
			{
				if (Row.c_str()[i] == EscapeChar && !Escaped)
				{
					Escaped = true;
				}
				else
				{
					Escaped = false;
				}
				i++;
			}
			i++;	// Skip over the qualifier
		}
		// Skip any white space
		while (Row.c_str()[i] == ' ' && i<Row.Length())
		{
			i++;
		}
		// Keep looking for delimiter
		Escaped = false;
		while ((Row.c_str()[i] != Delimiter || Escaped) && i<Row.Length())
		{
			if (Row.c_str()[i] == EscapeChar && !Escaped)
			{
				Escaped = true;
			}
			else
			{
				Escaped = false;
			}
			i++;
		}
		i++;	// Skip over the delimiter

		Count++;
	}
	// Skip any white space
	while (Row.c_str()[i] == ' ' && i<Row.Length())
	{
		i++;
	}
// Now read next word
	AnsiString ReturnVal = "";
	bool Escaped = false;
	if (Qualifier && Row.c_str()[i] == Qualifier)
	{
		// Move though to the end of the qualified word.
		i++;
		while ((Row.c_str()[i] != Qualifier || Escaped) && i<Row.Length())
		{
			if (Row.c_str()[i] == EscapeChar && !Escaped)
			{
				Escaped = true;
			}
			else
			{
				Escaped = false;
				ReturnVal += Row.c_str()[i];
			}
			i++;
		}
		i++;	// Skip over the qualifier
	}
	// Skip any white space
	while (Row.c_str()[i] == ' ' && i<Row.Length())
	{
		i++;
	}
	// Keep looking for delimiter
	Escaped = false;
	while ((Row.c_str()[i] != Delimiter || Escaped) && i<Row.Length())
	{
		if (Row.c_str()[i] == EscapeChar && !Escaped)
		{
			Escaped = true;
		}
		else
		{
			Escaped = false;
			ReturnVal += Row.c_str()[i];
		}
		i++;
	}

	return ReturnVal;


/*	AnsiString ReturnVal = "";
	bool LastCharEscape = false;
	int DelimiterCount = 0;
	for (int i=0; i<Row.Length(); i++)
	{
		// A delimiter, but not an escaped delimiter, therefore a new word is comming up.
		if (Row.c_str()[i] == Delimiter && !LastCharEscape)
		{
			DelimiterCount++;
			if (DelimiterCount > ACol)
			{
				break;
			}
			else
			{
				ReturnVal = "";
			}
		}
		// An EscapeChar is found. Has it been escaped itself?
		else if (Row.c_str()[i] == EscapeChar)
		{
			// Already Escaped, therefore it's a legit char.
			if (LastCharEscape)
			{
				LastCharEscape = false;
				ReturnVal += Row.c_str()[i];
			}
			// Just an escape...
			else
			{
				LastCharEscape = true;
			}
		}
		// Just a normal char...
		else
		{
			ReturnVal += Row.c_str()[i];
			LastCharEscape = false;
		}
	}
	return ReturnVal;*/
}
//---------------------------------------------------------------------------

