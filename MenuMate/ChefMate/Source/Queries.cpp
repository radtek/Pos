//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>

#include "Queries.h"
#include <Memory>

#pragma package(smart_init)

//---------------------------------------------------------------------------

int GetTotalDailyOrders(Database::TDBTransaction &DBTransaction)
{
	int Total = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
			"SELECT "
			" COUNT (TIME_KEY)"
			" FROM"
			" TURNAROUNDTIMES"
			" WHERE SALE_START_TIME IS NOT NULL AND"
			" SALE_END_TIME IS NOT NULL AND"
			" SALE_START_TIME > :START_TIME AND"
			" SALE_END_TIME < :SALE_END_TIME";
		TDateTime DateTime = Now();
		if(double(DateTime - int(DateTime)) < double(1/24*5) ) // Before 5am working in yesterday.
		{
			IBInternalQuery->ParamByName("START_TIME")->AsDateTime = (Date() - 1.0 + double(1.0/24.0*5.0));
			IBInternalQuery->ParamByName("SALE_END_TIME")->AsDateTime = (Date() + double(1.0/24.0*5.0));
		}
		else // after 5 am working in today.
		{
			IBInternalQuery->ParamByName("START_TIME")->AsDateTime = (Date() + double(1.0/24.0*5.0));
			IBInternalQuery->ParamByName("SALE_END_TIME")->AsDateTime = (Date() + double(1.0 + 1.0/24.0*5.0));
		}
		IBInternalQuery->ExecQuery();
		Total = IBInternalQuery->FieldByName("COUNT")->AsInteger;
	}
	catch(Exception &E)
	{

	}
	return Total;
}

TDateTime GetSalesAverage(Database::TDBTransaction &DBTransaction)
{
	TDateTime Total = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
			"SELECT "
			" AVG (SALE_END_TIME - SALE_START_TIME) AVGERAGE"
			" FROM"
			" TURNAROUNDTIMES"
			" WHERE SALE_START_TIME IS NOT NULL AND"
			" SALE_END_TIME IS NOT NULL AND"
			" SALE_START_TIME > :SALE_START_TIME";
		TDateTime DateTime = Now();
		DateTime = DateTime - double((1.0/24.0)/2.0);
		IBInternalQuery->ParamByName("SALE_START_TIME")->AsDateTime = (DateTime);
		IBInternalQuery->ExecQuery();
		Total = IBInternalQuery->FieldByName("AVGERAGE")->AsFloat;
	}
	catch(Exception &E)
	{

	}
	return Total;
}

TDateTime GetMakeAverage(Database::TDBTransaction &DBTransaction)
{
	TDateTime Total = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
			"SELECT "
			" AVG (MAKE_END_TIME - MAKE_START_TIME) AVGERAGE"
			" FROM"
			" TURNAROUNDTIMES"
			" WHERE MAKE_START_TIME IS NOT NULL AND"
			" MAKE_END_TIME IS NOT NULL AND"
			" MAKE_START_TIME > :MAKE_START_TIME";
		TDateTime DateTime = Now();
		DateTime = DateTime - double((1.0/24.0)/2.0);
		IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (DateTime);

		//Code From 5am to 5 pm.
	/*   if(double(DateTime - int(DateTime)) < double(1/24*5) ) // Before 5am working in yesterday.
		{
			IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (Date() - 1.0 + double(1.0/24.0*5.0));
			IBInternalQuery->ParamByName("MAKE_END_TIME")->AsDateTime = (Date() + double(1.0/24.0*5.0));
		}
		else // after 5 am working in today.
		{
			IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (Date() + double(1.0/24.0*5.0));
			IBInternalQuery->ParamByName("MAKE_END_TIME")->AsDateTime = (Date() + double(1.0 + 1.0/24.0*5.0));
		}*/
		IBInternalQuery->ExecQuery();
		Total = IBInternalQuery->FieldByName("AVGERAGE")->AsFloat;
	}
	catch(Exception &E)
	{

	}
	return Total;
}

TDateTime GetMakeLongest(Database::TDBTransaction &DBTransaction)
{
	TDateTime Total = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
			"SELECT "
			" MAX (MAKE_END_TIME - MAKE_START_TIME) MAXIMUM"
			" FROM"
			" TURNAROUNDTIMES"
			" WHERE MAKE_START_TIME IS NOT NULL AND"
			" MAKE_END_TIME IS NOT NULL AND"
			" MAKE_START_TIME > :MAKE_START_TIME AND"
			" MAKE_END_TIME < :MAKE_END_TIME";
		TDateTime DateTime = Now();
		if(double(DateTime - int(DateTime)) < double(1/24*5) ) // Before 5am working in yesterday.
		{
			IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (Date() - 1.0 + double(1.0/24.0*5.0));
			IBInternalQuery->ParamByName("MAKE_END_TIME")->AsDateTime = (Date() + double(1.0/24.0*5.0));
		}
		else // after 5 am working in today.
		{
			IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (Date() + double(1.0/24.0*5.0));
			IBInternalQuery->ParamByName("MAKE_END_TIME")->AsDateTime = (Date() + double(1.0 + 1.0/24.0*5.0));
		}
		IBInternalQuery->ExecQuery();
		Total = IBInternalQuery->FieldByName("MAXIMUM")->AsFloat;
	}
	catch(Exception &E)
	{
	}
	return Total;
}

int CloseMakeStartTime(Database::TDBTransaction &DBTransaction,int TimeKey)
{
	int RecordsUpdated = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(TimeKey != 0)
		{
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->ParamCheck = true;
			IBInternalQuery->SQL->Text =
			"UPDATE TURNAROUNDTIMES "
			"SET "
				"MAKE_END_TIME = :MAKE_END_TIME "
			"WHERE "
				"TIME_KEY	= :TIME_KEY";
			IBInternalQuery->ParamByName("MAKE_END_TIME")->AsDateTime = (Now());
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			RecordsUpdated = IBInternalQuery->RowsAffected;
		}
	}
	catch(Exception &E)
	{
		ShowMessage("Close Time Failed");
	}
	return RecordsUpdated;
}

bool OutStandingOrders(Database::TDBTransaction &DBTransaction,TDateTime Minutes)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
			"SELECT "
			" MIN (MAKE_START_TIME) MAXIMUM"
			" FROM"
			" TURNAROUNDTIMES"
			" WHERE MAKE_START_TIME IS NOT NULL AND"
			" MAKE_END_TIME IS NULL AND"
			" MAKE_START_TIME < :MAKE_START_TIME ";
		TDateTime DateTime = Now() - Minutes;
		IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (DateTime);
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
	}
	return RetVal;
}

bool GetOutStandingOrder(Database::TDBTransaction &DBTransaction,TDateTime Minutes,TListChefDisplay *List)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
			"SELECT "
			" KITCHENDOCKETS.*, TURNAROUNDTIMES.MAKE_START_TIME MAXIMUM,TURNAROUNDTIMES.TIME_KEY "
			" FROM"
			" KITCHENDOCKETS"
			" LEFT JOIN TURNAROUNDTIMES ON TURNAROUNDTIMES.TIME_KEY = KITCHENDOCKETS.TIME_KEY"
			" WHERE TURNAROUNDTIMES.MAKE_START_TIME IS NOT NULL AND"
			" TURNAROUNDTIMES.MAKE_END_TIME IS NULL AND"
			" TURNAROUNDTIMES.MAKE_START_TIME < :MAKE_START_TIME "
			" ORDER BY TURNAROUNDTIMES.MAKE_START_TIME";
		TDateTime DateTime = Now() - Minutes;
		IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = (DateTime);
		IBInternalQuery->ExecQuery();
		for (int i = 0; !IBInternalQuery->Eof && i < 10 ; IBInternalQuery->Next(), i++)
		{
			RetVal = true;
			int TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;

			TNetMessageChefMate *ChefRequest = new TNetMessageChefMate;
			ChefRequest->OrderNumber = IBInternalQuery->FieldByName("DOCKETS_KEY")->AsInteger;
			ChefRequest->TillColour = clRed;
			ChefRequest->tmPrinted = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			ChefRequest->FinalOrder = true;
			ChefRequest->RemoveOrder = true;
			ChefRequest->DisplayEnabled = false;
			ChefRequest->Received = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			ChefRequest->ChitNumber = IBInternalQuery->FieldByName("CHITNUMBER")->AsString;
			ChefRequest->Device = "Aged " + IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
			ChefRequest->User = "Docket No. " + IntToStr(IBInternalQuery->FieldByName("DOCKETS_NUMBER")->AsInteger);
			ChefRequest->TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;

			std::auto_ptr<TMemoryStream> Docket(new TMemoryStream);
			std::auto_ptr<TStringList> StringList(new TStringList);

			Docket->Clear();
			IBInternalQuery->FieldByName("DOCKET")->SaveToStream(Docket.get());
			Docket->Position = 0;
			if(Docket->Size > 0)
			{
				StringList->Add("!!!! WARNING AGED ORDER !!!!");
				StreamToStringList(Docket.get(),StringList.get());
			}
			else
			{
				StringList->Add("!!!! WARNING AGED ORDER !!!!");
				StringList->Add("<No Kitchen Docket Found>");
				StringList->Add(ChefRequest->User);
				StringList->Add(ChefRequest->Device);
				StringList->Add("Printed :" + ChefRequest->tmPrinted.FormatString("dd/mm/yy hh:nn:ss"));
			}
			ChefRequest->Order->Clear();
			ChefRequest->Order->Assign(StringList.get());
			List->Add(ChefRequest);
		}
	}
	catch(Exception &E)
	{
	}
	return RetVal;
}

void StreamToStringList(TMemoryStream *Stream, TStringList *Lines)
{
//---------------------------------------------------------------------------
// 					EPSON PRINTER CONTROL LANGUAGE COMMANDS
//---------------------------------------------------------------------------
// Prefixes for most of the following commands
const char ESC							= 0x1B;
const char FS 							= 0x1C;
const char GS 							= 0x1D;
//---------------------------------------------------------------------------
// Doesn't reset the downloaded graphic.
const char RESET						= 0x40;	// '@' Use ESC @
//---------------------------------------------------------------------------
// Chars
//---------------------------------------------------------------------------
// Characters for drawing a single or double line using Epson fonts.
const char FONT_SOLID_LINE			= 0xC4;
const char FONT_DOUBLE_LINE		= 0xCD;
//---------------------------------------------------------------------------
const char CR							= 0x0D;	// Carriage Return
const char LF							= 0x0A;	// Line Feed
const char FF							= 0x0C;	// Form Feed
//---------------------------------------------------------------------------
// Font Commands
//---------------------------------------------------------------------------
// Font size selector.
const char PRINT_MODE				= 0x21;	// '!'
const char COLOUR_MODE				= 0x72;	// '!'
// The mode bits constitute the 3rd part of the Font Command (ESC !) sequence
	const char MODE_FONT_A			= 0x00;
	const char MODE_FONT_B			= 0x01;
	const char MODE_BOLD				= 0x08;
	const char MODE_DOUBLE_HEIGHT	= 0x10;
	const char MODE_DOUBLE_WIDTH	= 0x20;
	const char MODE_UNDERLINE		= 0x80;
//---------------------------------------------------------------------------
// Bar Code Font Commands
//---------------------------------------------------------------------------
	const char CODE_FONT_A[]			= { 0x7B, 0x41};// {A
	const char CODE_FONT_B[]			= { 0x7B, 0x42};// {B
	const char CODE_FONT_C[]			= { 0x7B, 0x43};// {C
//---------------------------------------------------------------------------
// Cut commands
//---------------------------------------------------------------------------
const char CUT_MODE					= 0x56;	// 'V'
// The mode bits constitute the 3rd part of the Cut Command (GS V) sequence
// Follow with 0x00 -> 0xFF to scroll before cut
	const char MODE_FULL_CUT		= 0x41;
	const char MODE_PARTIAL_CUT	= 0x42;
//---------------------------------------------------------------------------
// Graphic commands
//---------------------------------------------------------------------------
const char DOWNLOAD_GRAPHIC		= 0x2A;	// '*'
// Two bytes following are Width/8 (m) and and Height/8 (n). ie (GS '*' m n)
// followed by m*n*8 data bytes. Thes are aranged vertically, where each
// pixel corresponds to a bit. MSB at the top, LSB at the bottom, followed
// by the rest of n bytes. Then back up to the top for the n+1 byte at the
// m=1 pos.
// Max m = 72. Max n = 48. Max m*n = 1536 for Axiohm A715
const char PRINT_GRAPHIC			= 0x2F;	// '/'
// Prints the graphic stored by DOWNLOAD_GRAPHIC in the following fashon.
// Use (GS, '/', Width) where Width is one of the following.
	const char SINGLE_HEIGHT_SINGLE_WIDTH	= 0x00;	// 0x30
	const char SINGLE_HEIGHT_DOUBLE_WIDTH	= 0x01;	// 0x31
	const char DOUBLE_HEIGHT_SINGLE_WIDTH	= 0x02;	// 0x32
	const char DOUBLE_HEIGHT_DOUBLE_WIDTH	= 0x03;	// 0x33
//---------------------------------------------------------------------------
// Barcode commands
//---------------------------------------------------------------------------
const char BARCODE_HRI_POSITION				= 0x48; // 'H'
// Human Readable Interpretation. (GS 'H' n) where n is 1 of the 4 following
// positions.
	const char BARCODE_HRI_POSITION_NONE	= 0x00;
	const char BARCODE_HRI_POSITION_ABOVE	= 0x01;
	const char BARCODE_HRI_POSITION_BELOW	= 0x02;
	const char BARCODE_HRI_POSITION_BOTH	= 0x03;

const char BARCODE_HRI_FONT					= 0x66; // 'f'
// Font for Human Readable Interpretation. (GS 'f' n) where n is 1 of the 2
// font types.
	const char BARCODE_HRI_FONT_A				= 0x00;
	const char BARCODE_HRI_FONT_B				= 0x01;

const char BARCODE_RATIO						= 0x67; // 'g'
// Bar code width (raito of Wide:Narrow) (GS 'g' n) where n is 1 of the 3
// font types.
	const char BARCODE_RATION_2_1				= 0x00;
	const char BARCODE_RATION_3_1				= 0x01;
	const char BARCODE_RATION_5_2				= 0x02;

const char BARCODE_AUTO_CHECK_CHAR			= 0x63; // 'c'

const char BARCODE_HEIGHT						= 0x68; // 'h'
// Vertical height in dots. (GS 'h' n) where n = 00 to FF
const char BARCODE_PRINT						= 0x6B; // 'k'
// Using option 2. (GS 'k' m n {d1 d2 d3...dn})
// n = number of data characters
// d is bar code data
// m is one of the below bar code types.
	const char BARCODE_TYPE_UPC_A				= 0x41;	// 11 to 12 chars, 48 to 57
	const char BARCODE_TYPE_UPC_E				= 0x42;	// 11 to 12 chars, 48 to 57
	const char BARCODE_TYPE_EAN13				= 0x43;	// 12 to 13 chars, 48 to 57
	const char BARCODE_TYPE_EAN8				= 0x44;	// 7 to 8 chars, 48 to 57
	const char BARCODE_TYPE_CODE_39			= 0x45;	// 1 to 255 chars, 48 to 57, 65 to 90, 32, 36, 37, 43, 45, 46, 47
	const char BARCODE_TYPE_INTERLEAVED		= 0x46;	// 1 to 255 chars, 48 to 57 (even # chars)
	const char BARCODE_TYPE_CODABAR			= 0x47;	// 1 to 255 chars, 48 to 57, 65 to 68, 36, 43, 45, 46, 47, 58
	const char BARCODE_TYPE_CODE_93			= 0x48;	// 1 to 255 chars
	const char BARCODE_TYPE_CODE_128			= 0x49;	// 2 to 255 chars

const char BARCODE_HORZ_EXPANSION			= 0x77; // 'w'
// Horizontal size. (GS 'w' n) where n = 01 to 04
//---------------------------------------------------------------------------
// Kanji Commands
//---------------------------------------------------------------------------
const char KANJI_CODE				= 0x43;	// 'C'
// Kanji code system. (FS 'C' n) where n = (00 or 48) or (01 or 49)
// In JIS mode,
//		Prmary byte:		<21>H to <7E>H
//		Secondary byte:	<21>H to <7E>H
// In SHIFT JIS mode,
//		Prmary byte:		<81>H to <9F>H and <E0>H to <EF>H
//		Secondary byte:	<40>H to <7E>H and <80>H to <FC>H
	const char KANJI_JIS				= 0x00;
	const char KANJI_SJIS			= 0x01;

const char KANJI_MODE_ON			= 0x26;	// '&'
// Select Kanji mode. (FS '&')
const char KANJI_MODE_OFF			= 0x2E;	// '.'
// Select Kanji mode. (FS '.')

	Stream->Position = 0;
	UnicodeString TrimmedLine;
	char *Line = (char *)Stream->Memory;
	for(int j = 0; j < Stream->Size; j++)
	{
		if (Line[j] == ESC)
		{
			if (Line[j+1] == PRINT_MODE)				j += 2;
			else if (Line[j+1] == COLOUR_MODE)		j += 2;
			else if (Line[j+1] == '@')						j = Stream->Size;
		}
		else if (Line[j] == GS)
		{
			if (Line[j+1] == CUT_MODE)					j += 2;
			else if (Line[j+1] == PRINT_GRAPHIC)	j += 2;
		}
		else if (Line[j] == CR)
		{
			if (Line[j+1] == LF)    					   j += 1;
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		}
		else if (Line[j] == LF)
		{
			if (Line[j+1] == CR)    					   j += 1;
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		}
		else
		{
			if(Line[j] == char(196))
			{
				TrimmedLine += "_";
			}
			else
			{
				TrimmedLine += Line[j];
			}
		}
	}
	if(TrimmedLine != "")
		Lines->Add(TrimmedLine);
}


