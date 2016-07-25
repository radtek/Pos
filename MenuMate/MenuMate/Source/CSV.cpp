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
//	Qualifier 	= "";
	EscapeChar	= '\\';
}
//---------------------------------------------------------------------------
TCsv::~TCsv()
{
	delete fStrings;
}
//---------------------------------------------------------------------------
void TCsv::LoadFromFile(const UnicodeString FileName)
{
	fStrings->LoadFromFile(FileName);
}
//---------------------------------------------------------------------------
void TCsv::SaveToFile(const UnicodeString FileName)
{
	fStrings->SaveToFile(FileName);
}

//---------------------------------------------------------------------------
bool TCsv::AppendToFile(const UnicodeString FileName)
{
	bool Success = false;
	__int64 Time = ::GetTickCount() + 10000;
	while (!Success && Time > ::GetTickCount())
	{
		TFileStream *Stream = NULL;
		try
		{

			if(FileExists(FileName))
			{
				Stream = new TFileStream(FileName,fmOpenReadWrite | fmShareExclusive);
			}
			else
			{
				Stream = new TFileStream(FileName,fmCreate | fmShareExclusive);
			}
			Stream->Seek(0,soFromEnd);
			fStrings->SaveToStream(Stream);
			delete Stream;
			Success = true;			
		}
		catch(Exception &Err)
		{
			delete Stream;
			Success = false;
		}
	}
	return Success;
}

//---------------------------------------------------------------------------
int TCsv::GetRowCount()
{
	return fStrings->Count;
}
//---------------------------------------------------------------------------
int TCsv::GetColCount(int ARow)
{
	UnicodeString Row = fStrings->Strings[ARow];

	bool LastCharEscape = false;
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
	return DelimiterCount + 1;
}
//---------------------------------------------------------------------------
UnicodeString TCsv::GetCells(int ACol, int ARow)
{
	return ColumnText(fStrings->Strings[ARow], ACol);
}
//---------------------------------------------------------------------------
void TCsv::SetCells(int ACol, int ARow, const UnicodeString Value)
{
	UnicodeString Row = fStrings->Strings[ARow];

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
	UnicodeString EscapedValue = "";
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
int TCsv::Add(const UnicodeString Value)
{
	return fStrings->Add(Value);
}

UnicodeString TCsv::GetRow(int Value)
{
	UnicodeString RetVal = "";
	if(Value < fStrings->Count)
	{
		RetVal = fStrings->Strings[Value];
	}
	return RetVal;
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
UnicodeString TCsv::ColumnText(UnicodeString Row, int ACol)
{
	UnicodeString ReturnVal = "";
	bool LastCharEscape = false;
	int DelimiterCount = 0;
	int RowIndex = 0;
	for (; RowIndex<Row.Length(); RowIndex++)
	{
		// A delimiter, but not an escaped delimiter, therefore a new word is comming up.
		if (Row.c_str()[RowIndex] == Delimiter && !LastCharEscape)
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
		else if (Row.c_str()[RowIndex] == EscapeChar)
		{
			// Already Escaped, therefore it's a legit char.
			if (LastCharEscape)
			{
				LastCharEscape = false;
				ReturnVal += Row.c_str()[RowIndex];
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
			ReturnVal += Row.c_str()[RowIndex];
			LastCharEscape = false;
		}
	}

	// End of Row has no Delimiter.
	if(RowIndex == (Row.Length()))
	{
		DelimiterCount++;
	}

	if(DelimiterCount != (ACol + 1))
	{
		ReturnVal = "";
	}
	return ReturnVal;
}
//---------------------------------------------------------------------------


int TCsv::FindFirst(int ACol, UnicodeString Val)
{
   for (int i = 0 ; i < fStrings->Count; i++)
   {
      if(this->Cells[ACol][i] == Val)
      {
         return i;
      }
   }
   return -1;
}

void TCsv::Assign(TCsv * inData)
{
	if(inData != NULL)
	{
		for (int i = 0; i < inData->RowCount ; i++)
      {
			Add(inData->GetRow(i));
		}
	}
}
