// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Comms.h"
#include "DocketManager.h"
#include "PrintFormat.h"
#include "MMLogging.h"
#include "Enum.h"
#include "MMMessageBox.h"
#include "ReqPrintJob.h"
#include "DBSecurity.h"
TManagerDockets *ManagerDockets;
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerDockets::TManagerDockets(Database::TDBControl &inDBControl) : DBControl(inDBControl)
{
   Docket = new TMemoryStream;
   DocketToArchive = new TMemoryStream;
}

TManagerDockets::~TManagerDockets()
{
   delete Docket;
   delete DocketToArchive;
}

void TManagerDockets::Initialise(UnicodeString inTerminalName, int inTerminalDeviceKey)
{
   TerminalName = inTerminalName;
   TerminalDeviceKey = inTerminalDeviceKey;
}

bool TManagerDockets::ReceiptsExist()
{
   bool ReceiptsExist = false;

   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT DOCKETS_NUMBER FROM KITCHENDOCKETS WHERE TERMINAL_NAME = :TERMINAL_NAME";
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount == 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT DOCKETS_NUMBER FROM KITCHENDOCKETS WHERE TERMINAL_NAME = :TERMINAL_NAME";
		 IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
		 IBInternalQuery->ExecQuery();
		 if (IBInternalQuery->RecordCount)
		 {
			ReceiptsExist = true;
		 }
	  }
	  else
	  {
		 ReceiptsExist = true;
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ReceiptsExist;
}

bool TManagerDockets::IsEmpty()
{
   bool Retval = true;
   try
   {
	  Retval = Array.empty();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return Retval;
}

bool TManagerDockets::Eof()
{
   bool Retval = true;
   try
   {
	  Retval = (ArrayIndex >= Array.size() - 1);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return Retval;
}

bool TManagerDockets::Bof()
{
   bool Retval = true;
   try
   {
	  Retval = (ArrayIndex <= 0);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return Retval;
}

void TManagerDockets::First()
{
   try
   {
	  ArrayIndex = 0;
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  Get(DBTransaction);
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void TManagerDockets::Next()
{
   try
   {
	  ArrayIndex++;
	  if (ArrayIndex >= Array.size())
	  {
		 ArrayIndex = Array.size() - 1;
	  }
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  Get(DBTransaction);
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void TManagerDockets::Prev()
{
   try
   {
	  ArrayIndex--;
	  if (ArrayIndex <= 0)
	  {
		 ArrayIndex = 0;
	  }
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  Get(DBTransaction);
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }

}

void TManagerDockets::GetLastReceipt(Database::TDBTransaction &DBTransaction)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT MAX(DOCKETS_KEY) FROM KITCHENDOCKETS WHERE TERMINAL_NAME = :TERMINAL_NAME";
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  IBInternalQuery->ExecQuery();
	  int key = IBInternalQuery->Fields[0]->AsInteger;
	  Array.clear();
	  Array.push_back(key);
	  Get(DBTransaction);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TManagerDockets::Open()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  UnicodeString Format = "Select * From KITCHENDOCKETS %0:s Order By DOCKETS_NUMBER Desc;";

	  UnicodeString SQLWhereText;
	  TVarRec args[1];
	  if (Filter == eThisTill)
	  {
		 SQLWhereText = " WHERE TERMINAL_NAME = :TERMINAL_NAME " "AND TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore "; //
	  }
	  else if (Filter == eActiveDockets)
	  {
		 SQLWhereText =
			 " LEFT JOIN TURNAROUNDTIMES ON TURNAROUNDTIMES.TIME_KEY = KITCHENDOCKETS.TIME_KEY" " WHERE TURNAROUNDTIMES.MAKE_START_TIME IS NOT NULL AND" " TURNAROUNDTIMES.MAKE_END_TIME IS NULL AND" " TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore ";
	  }
	  else
	  {
		 SQLWhereText = " WHERE TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore "; // Desc
	  }
	  args[0] = SQLWhereText;

	  UnicodeString Result = Format.Format(Format, args, 0);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = Result;
	  IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime = Date;
	  IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = Date + 1; // Midnight Tomorrow.;
	  if (Filter == eThisTill)
	  {
		 IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  }
	  IBInternalQuery->ExecQuery();

	  Array.clear();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 Array.push_back(IBInternalQuery->FieldByName("DOCKETS_KEY")->AsInteger);
	  }
	  DBTransaction.Commit();

	  First();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void TManagerDockets::Close()
{
}

// ---------------------------------------------------------------------------
bool TManagerDockets::Get(Database::TDBTransaction &DBTransaction)
{
   bool retVal = false;
   try
   {
	  if (!Array.empty())
	  {
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		 UnicodeString Format = "Select DOCKET From KITCHENDOCKETS WHERE DOCKETS_KEY = :DOCKETS_KEY;";

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = Format;
		 IBInternalQuery->ParamByName("DOCKETS_KEY")->AsInteger = Array[ArrayIndex];
		 IBInternalQuery->ExecQuery();

         DocketNumber=Array[ArrayIndex];
     	 Docket->Clear();
		 IBInternalQuery->FieldByName("DOCKET")->SaveToStream(Docket);
		 Docket->Position = 0;
		 retVal = true;
	  }
	  else
	  {
		 Docket->Clear();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return retVal;
}

void TManagerDockets::SetCurrentDate(TDateTime SelectedDate)
{
   try
   {
	  FSelectedDate = SelectedDate;
	  Open();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

bool TManagerDockets::FindChit(UnicodeString ChitNumber)
{
   bool RetVal = false;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT DOCKETS_KEY FROM KITCHENDOCKETS WHERE CHITNUMBER = :CHITNUMBER";
	  IBInternalQuery->ParamByName("CHITNUMBER")->AsString = ChitNumber;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount == 0) // no records
	  {
		 MessageBox("Docket Chit number not found.", "Docket Not Found", MB_ICONINFORMATION + MB_OK);
	  }
	  else
	  {
		 RetVal = true;
		 Array.clear();
		 Array.push_back(IBInternalQuery->FieldByName("DOCKETS_KEY")->AsInteger);
		 Get(DBTransaction);
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

bool TManagerDockets::FindBarcode(UnicodeString BarCode)
{
   bool RetVal = false;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT DOCKETS_KEY FROM KITCHENDOCKETS WHERE BARCODE = :BARCODE";
	  IBInternalQuery->ParamByName("BARCODE")->AsString = BarCode;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount == 0) // no records
	  {
		 MessageBox("Docket Barcode number not found.", "Docket Not Found", MB_ICONINFORMATION + MB_OK);
	  }
	  else
	  {
		 RetVal = true;
		 Array.clear();
		 Array.push_back(IBInternalQuery->FieldByName("DOCKETS_KEY")->AsInteger);
		 Get(DBTransaction);
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

bool TManagerDockets::Find(int DocketNumber)
{
   bool RetVal = false;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT DOCKETS_KEY FROM KITCHENDOCKETS WHERE DOCKETS_NUMBER = :DOCKETS_NUMBER";
	  IBInternalQuery->ParamByName("DOCKETS_NUMBER")->AsString = DocketNumber;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount == 0) // no records
	  {
		 MessageBox("Docket Number number not found.", "Docket Not Found", MB_ICONINFORMATION + MB_OK);
	  }
	  else
	  {
		 RetVal = true;
		 Array.clear();
		 Array.push_back(IBInternalQuery->FieldByName("DOCKETS_KEY")->AsInteger);
		 Get(DBTransaction);
	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TManagerDockets::Get(TStringList *Lines)
{
   try
   {
	  ManagerDockets->Docket->Position = 0;
	  UnicodeString TrimmedLine;
	  char *Line = (char*)ManagerDockets->Docket->Memory;
	  for (int j = 0; j < ManagerDockets->Docket->Size; j++)
	  {
		 if (Line[j] == ESC)
		 {
			if (Line[j + 1] == PRINT_MODE)
			   j += 2;
			else if (Line[j + 1] == COLOUR_MODE)
			   j += 2;
			else if (Line[j + 1] == STAR_OPEN_DRAWER)
			   j += 4;
			else if (Line[j + 1] == STAR_CUT_MODE)
			   j += 2;
			else if (Line[j + 1] == STAR_UNDERLINE)
			   j += 2;
			else if (Line[j + 1] == STAR_FONT_NORMAL_HEIGHT)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_DOUBLE_HEIGHT)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_A)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_B)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_INTENSITY_NORMAL)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_INTENSITY_BOLD)
			   j += 1;
		 }
		 else if (Line[j] == STAR_FONT_NORMAL_WIDTH || Line[j] == STAR_FONT_DOUBLE_WIDTH)
		 {
			// Single Character command just step over it.
		 }
		 else if (Line[j] == GS)
		 {
			if (Line[j + 1] == CUT_MODE)
			   j += 2;
			else if (Line[j + 1] == PRINT_GRAPHIC)
			   j += 2;
		 }
		 else if (Line[j] == ::CR)
		 {
			if (Line[j + 1] == ::LF)
			   j += 1;
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		 }
		 else if (Line[j] == ::LF)
		 {
			if (Line[j + 1] == ::CR)
			   j += 1;
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		 }
		 else
		 {
			if (Line[j] == char(196))
			{
			   TrimmedLine += "-";
			}
			else
			{
			   TrimmedLine += Line[j];
			}
		 }
	  }
	  if (TrimmedLine != "")
		 Lines->Add(TrimmedLine);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }

}

void TManagerDockets::GetWithSymbols(TStringList *Lines)
{
   try
   {
	  ManagerDockets->Docket->Position = 0;
	  UnicodeString TrimmedLine;
	  char *Line = (char*)ManagerDockets->Docket->Memory;
	  for (int j = 0; j < ManagerDockets->Docket->Size; j++)
	  {
		 if (Line[j] == ESC)
		 {
			TrimmedLine += "[ESC]";
			if (Line[j + 1] == PRINT_MODE)
			{
			   TrimmedLine += "[PRINT_MODE]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 2;
			}
			else if (Line[j + 1] == COLOUR_MODE)
			{
			   TrimmedLine += "[COLOUR_MODE]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 2;
			}
			else if (Line[j + 1] == STAR_OPEN_DRAWER)
			{
			   TrimmedLine += "[STAR_OPEN_DRAWER]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 4;
			}
			else if (Line[j + 1] == STAR_CUT_MODE)
			{
			   TrimmedLine += "[STAR_CUT_MODE]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 2;
			}
			else if (Line[j + 1] == STAR_UNDERLINE)
			{
			   TrimmedLine += "[STAR_UNDERLINE]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 2;
			}
			else if (Line[j + 1] == STAR_FONT_NORMAL_HEIGHT)
			{
			   TrimmedLine += "[STAR_FONT_NORMAL_HEIGHT]";
			   j += 1;
			}
			else if (Line[j + 1] == STAR_FONT_DOUBLE_HEIGHT)
			{
			   TrimmedLine += "[STAR_FONT_DOUBLE_HEIGHT]";
			   j += 1;
			}
			else if (Line[j + 1] == STAR_FONT_A)
			{
			   TrimmedLine += "[STAR_FONT_A]";
			   j += 1;
			}
			else if (Line[j + 1] == STAR_FONT_B)
			{
			   TrimmedLine += "[STAR_FONT_B]";
			   j += 1;
			}
			else if (Line[j + 1] == STAR_FONT_INTENSITY_NORMAL)
			{
			   TrimmedLine += "[STAR_FONT_INTENSITY_NORMAL]";
			   j += 1;
			}
			else if (Line[j + 1] == STAR_FONT_INTENSITY_BOLD)
			{
			   TrimmedLine += "[STAR_FONT_INTENSITY_BOLD]";
			   j += 1;
			}
		 }
		 else if (Line[j] == STAR_FONT_NORMAL_WIDTH || Line[j] == STAR_FONT_DOUBLE_WIDTH)
		 {
			TrimmedLine += "[STAR_FONT_WIDTH]";
			// Single Character command just step over it.
		 }
		 else if (Line[j] == GS)
		 {
			TrimmedLine += "[GS]";
			if (Line[j + 1] == CUT_MODE)
			{
			   TrimmedLine += "[CUT_MODE]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 2;
			}
			else if (Line[j + 1] == PRINT_GRAPHIC)
			{
			   TrimmedLine += "[PRINT_GRAPHIC]" + UnicodeString::Format(" %x", ARRAYOFCONST(((int)Line[j + 2])));
			   j += 2;
			}
		 }
		 else if (Line[j] == ::CR)
		 {
			TrimmedLine += "[CR]";
			if (Line[j + 1] == ::LF)
			{
			   TrimmedLine += "[LF]";
			   j += 1;
			}
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		 }
		 else if (Line[j] == ::LF)
		 {
			TrimmedLine += "[LF]";
			if (Line[j + 1] == ::CR)
			{
			   TrimmedLine += "[CR]";
			   j += 1;
			}
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		 }
		 else
		 {
			if (Line[j] == char(196))
			{
			   TrimmedLine += "-";
			}
			else
			{
			   TrimmedLine += Line[j];
			}
		 }
	  }
	  if (TrimmedLine != "")
		 Lines->Add(TrimmedLine);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }

}

void TManagerDockets::PrintLastDocket()
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("There is no Receipt printer selected.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {
	  TPrintout *Printout = new TPrintout;
	  Printout->Printer = TComms::Instance().ReceiptPrinter;
	  try
	  {
		 if (DocketToArchive->Size != 0)
		 {
			DocketToArchive->Position = 0;
			Printout->PrintToPrinterStream(DocketToArchive, TComms::Instance().ReceiptPrinter.UNCName());
		 }
		 else
  		 {
			Database::TDBTransaction DBTransaction(DBControl);
	  		DBTransaction.StartTransaction();
			GetLastReceipt(DBTransaction);
			DBTransaction.Commit();

			Docket->Position = 0;
			Printout->PrintToPrinterStream(Docket, TComms::Instance().ReceiptPrinter.UNCName());
		 }
	  }
	  __finally
	  {
		 delete Printout;
	  }
   }
}

void TManagerDockets::Print()
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("There is no Receipt printer selected.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {

      Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

      TPrintout *Printout = new TPrintout;

	  try
	  {

	  Printout->Printer = TComms::Instance().ReceiptPrinter;
       Printout->PrintToPrinterStream(Docket, TComms::Instance().ReceiptPrinter.UNCName());


	  int reprint_order_key;

      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
      IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_REPRINTORDER_KEY, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  reprint_order_key = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text ="INSERT INTO REPRINTORDER (" "REPRINTORDER_KEY, " "DOCKETS_NUMBER, " "USERNAME, " "TERMINAL_NAME, " "TIME_STAMP) " "VALUES ("
       ":REPRINTORDER_KEY, " ":DOCKETS_NUMBER, " ":USERNAME, " ":TERMINAL_NAME, " ":TIME_STAMP);";



	  IBInternalQuery->ParamByName("REPRINTORDER_KEY")->AsInteger = reprint_order_key;
	  IBInternalQuery->ParamByName("DOCKETS_NUMBER")->AsInteger = DocketNumber;
	  IBInternalQuery->ParamByName("USERNAME")->AsString = TDeviceRealTerminal::Instance().User.LatestLoginName;
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString =TDeviceRealTerminal::Instance().ID.Name;
	  IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();

	  IBInternalQuery->ExecQuery();

	  }
	  __finally
	  {
		 delete Printout;
	  }
       DBTransaction.Commit();

   }
}

int TManagerDockets::GetNextDocketNumber()
{
   int RetVal = 0;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DOCKETSNUMBER, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  CurrentDocketNumber = RetVal = IBInternalQuery->Fields[0]->AsInteger;
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TManagerDockets::Archive(TReqPrintJob * Request)
{
   try
   {
	  std::auto_ptr <TStringList> StringReceipt(new TStringList);
	  for (int i = 0; i < Request->Printouts->Count; i++)
	  {
		 TPrintout *Printout = (TPrintout*)Request->Printouts->Items[i];
		 int DocketNumber = StrToIntDef(Printout->PrintInfo["DocketNumber"], 0);
		 if (DocketNumber != 0)
		 {
			UnicodeString ChitNumber = Printout->PrintInfo["ChitNumber"];
			UnicodeString BarCode = Printout->PrintInfo["Barcode"];
			int CurrentTimeKey = StrToIntDef(Printout->PrintInfo["CurrentTimeKey"], 0);

			DocketToArchive->Clear();
			StringReceipt->Clear();

			Printout->PrintToStrings(StringReceipt.get());

			DocketToArchive->Position = 0;
			StringReceipt->SaveToStream(DocketToArchive);
			DocketToArchive->Position = 0;

			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_KITCHENDOCKETS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int Key = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO KITCHENDOCKETS (" "DOCKETS_KEY, " "DOCKETS_NUMBER, " "NAME, " "BARCODE, " "CHITNUMBER, "
				"TIME_STAMP, " "TIME_KEY, " "TERMINAL_NAME, " "DOCKET) " "VALUES (" ":DOCKETS_KEY, " ":DOCKETS_NUMBER, " ":NAME, "
				":BARCODE, " ":CHITNUMBER, " ":TIME_STAMP, " ":TIME_KEY, " ":TERMINAL_NAME, " ":DOCKET) ";
			IBInternalQuery->ParamByName("DOCKETS_KEY")->AsInteger = Key;
			IBInternalQuery->ParamByName("DOCKETS_NUMBER")->AsInteger = DocketNumber;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = CurrentTimeKey;
			IBInternalQuery->ParamByName("NAME")->AsString = "";
			IBInternalQuery->ParamByName("BARCODE")->AsString = BarCode.SubString(1, 20);
			IBInternalQuery->ParamByName("CHITNUMBER")->AsString = ChitNumber.SubString(1, 20);
			IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
			IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
			DocketToArchive->Position = 0;
			IBInternalQuery->ParamByName("DOCKET")->LoadFromStream(DocketToArchive);
			IBInternalQuery->ExecQuery();
			DBTransaction.Commit();
		 }
	  }
   }
   catch(Exception & Err)
   {
	  Request->Header.ErrorMsg = Err.Message;
	  Request->Header.Error = Err_ErrorMsg;
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "TManagerDockets::Archive " + Err.Message);
   }
}

void TManagerDockets::getOrderInfo(Database::TDBTransaction &DBTransaction)
{

   try
   {
	  if (!Array1.empty())
	  {
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		 AnsiString Format;
		 if (Array1[ArrayIndex].first == 1)
		 {
			Format = "Select first(1) RECEIPT,INVOICE_NUMBER, TOTAL , SECURITY_REF From ARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY;";
		 }
		 else
		 {
			Format = "Select first(1) RECEIPT,INVOICE_NUMBER, TOTAL , SECURITY_REF From DAYARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY;";
		 }

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = Format;
		 IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Array1[ArrayIndex].second;
		 IBInternalQuery->ExecQuery();

		 Docket->Clear();
		 IBInternalQuery->FieldByName("RECEIPT")->SaveToStream(Docket);
                 ReceiptValue = IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                 Sec_Ref= IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
                 InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
		 Docket->Position = 0;

	  }
	  else
	  {
		 Docket->Clear();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }

}


void TManagerDockets::OrderedArrayKey(AnsiString InvoiceNo)
{

   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT first(1) ARCBILL_KEY FROM DAYARCBILL WHERE INVOICE_NUMBER = :INVOICE_NUMBER";
	  IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNo;
	  IBInternalQuery->ExecQuery();
	  int key = IBInternalQuery->Fields[0]->AsInteger;
	  if (key == 0) // no records
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT first(1) ARCBILL_KEY FROM ARCBILL WHERE INVOICE_NUMBER = :INVOICE_NUMBER";
		 IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNo;
		 IBInternalQuery->ExecQuery();
		 key = IBInternalQuery->Fields[0]->AsInteger;
		 if (key != 0) // no records
		 {
			Array1.clear();
			Array1.push_back(TableKeyPair(1, key));
			getOrderInfo(DBTransaction);

		 }
	  }
	  else
	  {
		 Array1.clear();
		 Array1.push_back(TableKeyPair(2, key));
		 getOrderInfo(DBTransaction);

	  }
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }


}
