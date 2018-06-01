// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Comms.h"
#include "ReceiptManager.h"
#include "MMLogging.h"
#include "Enum.h"
#include "MMMessageBox.h"
#include "ManagerPhysicalPrinter.h"
#include "Printout.h"
#include "MallExportUpdateAdaptor.h"
#include "DBSecurity.h"
#include "PosMain.h"
#include "StringTools.h"
#include "FiscalDataUtility.h"
TManagerReceipt *ManagerReceipt;
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerReceipt::TManagerReceipt(Database::TDBControl &inDBControl) : DBControl(inDBControl)
{
   Receipt = new TMemoryStream;
   ReceiptToArchive = new TMemoryStream;
   ArrayIndex = 0;
}

TManagerReceipt::~TManagerReceipt()
{
   delete Receipt;
   delete ReceiptToArchive;
}

void TManagerReceipt::Initialise(AnsiString inTerminalName, int inTerminalDeviceKey)
{
   TerminalName = inTerminalName;
   TerminalDeviceKey = inTerminalDeviceKey;
}

bool TManagerReceipt::ReceiptsExist()
{
   bool ReceiptsExist = false;

   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();

     IBInternalQuery->SQL->Text = "select a.INVOICE_NUMBER from DAYARCBILL a"
                           " Where a.TERMINAL_NAME = :TERMINAL_NAME  and a.ARCBILL_KEY not in"
                           " (Select b.ARCBILL_KEY from DAYARCHIVE b"
                           " left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                           " where c.DISCOUNT_GROUPNAME = 'Non-Chargeable' or c.DISCOUNT_GROUPNAME = 'Complimentary')"
                           " group by 1";

	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount == 0)
	  {
		 IBInternalQuery->Close();
         IBInternalQuery->SQL->Text = " select a.INVOICE_NUMBER  from ARCBILL a "
                                   " Where a.TERMINAL_NAME = :TERMINAL_NAME  and a.ARCBILL_KEY not in"
                                   " (Select b.ARCBILL_KEY from  ARCHIVE b "
                                   " left join ARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                                   " where c.DISCOUNT_GROUPNAME = 'Non-Chargeable' or c.DISCOUNT_GROUPNAME = 'Complimentary') "
                                   " group by 1 ";
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

bool TManagerReceipt::IsEmpty()
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

bool TManagerReceipt::Eof()
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

bool TManagerReceipt::Bof()
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

void TManagerReceipt::First()
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

void TManagerReceipt::Next()
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

void TManagerReceipt::Prev()
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

void TManagerReceipt::GetLastReceipt(Database::TDBTransaction &DBTransaction)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = " select MAX(a.ARCBILL_KEY)  from DAYARCBILL a "
                                    " Where a.ARCBILL_KEY not in "
                                   " (Select b.ARCBILL_KEY from  DAYARCHIVE b "
                                     " left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                                     " where c.DISCOUNT_GROUPNAME = 'Non-Chargeable' or c.DISCOUNT_GROUPNAME = 'Complimentary') "
                                    " and RECEIPT is not null and a.TERMINAL_NAME = :TERMINAL_NAME  ";

	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  IBInternalQuery->ExecQuery();
	  int key = IBInternalQuery->Fields[0]->AsInteger;
	  if (key == 0) // no records
	  {
		 IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = " select MAX(a.ARCBILL_KEY)  from ARCBILL a "
                                         " Where a.ARCBILL_KEY not in "
                                        " (Select b.ARCBILL_KEY from  ARCHIVE b "
                                        " left join ARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                                         " where c.DISCOUNT_GROUPNAME = 'Non-Chargeable' or c.DISCOUNT_GROUPNAME = 'Complimentary') "
                                          " and RECEIPT is not null and a.TERMINAL_NAME = :TERMINAL_NAME  " ;
		 IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
		 IBInternalQuery->ExecQuery();
		 key = IBInternalQuery->Fields[0]->AsInteger;
		 Array.clear();
		 Array.push_back(TableKeyPair(1, key));
	  }
	  else
	  {
		 Array.clear();
		 Array.push_back(TableKeyPair(2, key));
	  }
      ArrayIndex =0;
	  Get(DBTransaction);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TManagerReceipt::Open()
{

   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   try
   {

      AnsiString Format = "SELECT 1 TABLETYPE, a.ARCBILL_KEY, a.TIME_STAMP From ARCBILL a "
                           "Where a.ARCBILL_KEY not in "
                           " ( Select b.ARCBILL_KEY from ARCHIVE b "
                            " left join ARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                            " where c.DISCOUNT_GROUPNAME = 'Non-Chargeable' or "
                            " c.DISCOUNT_GROUPNAME = 'Complimentary') %0:s "
                           "GROUP BY TABLETYPE,ARCBILL_KEY,TIME_STAMP "
             "Union All "
             "Select 2 TABLETYPE, a.ARCBILL_KEY, a.TIME_STAMP From DAYARCBILL a "
             "Where a.ARCBILL_KEY not in "
             "(Select b.ARCBILL_KEY from DAYARCHIVE b "
             "left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
             "where c.DISCOUNT_GROUPNAME = 'Non-Chargeable' or c.DISCOUNT_GROUPNAME = 'Complimentary') %0:s "
             "group by TABLETYPE,ARCBILL_KEY,TIME_STAMP "
             "Order By 3 Desc; " ;


	  AnsiString SQLWhereText;
	  TVarRec args[1];
	  if (!AllTerminals)
	  {
		 SQLWhereText += " AND  RECEIPT is not null AND a.TERMINAL_NAME = :TERMINAL_NAME " "AND a.TIME_STAMP > :TimeStampAfter AND a.TIME_STAMP < :TimeStampBefore "; //
	  }
	  else
	  {
		 SQLWhereText += " AND  RECEIPT is not null AND a.TIME_STAMP > :TimeStampAfter AND a.TIME_STAMP < :TimeStampBefore "; // Desc
	  }
	  args[0] = SQLWhereText;

	  AnsiString Result = Format.Format(Format, args, 0);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = Result;
	  IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime = Date;
	  IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = Date + 1; // Midnight Tomorrow.;
	  if (!AllTerminals)
	  {
		 IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  }
	  IBInternalQuery->ExecQuery();

	  Array.clear();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 Array.push_back(TableKeyPair(IBInternalQuery->FieldByName("TABLETYPE")->AsInteger,
			   IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger));
	  }
	  DBTransaction.Commit();

	  First();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

/*
void TManagerReceipt::Open(Database::TDBTransaction &DBTransaction)
{


TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

try
{
AnsiString Format = "Select ARCBILL_KEY,TIME_STAMP,INVOICE_NUMBER,RECEIPT From ARCBILL %0:s "
"Union All "
"Select ARCBILL_KEY,TIME_STAMP,INVOICE_NUMBER,RECEIPT From DAYARCBILL %0:s Order By 2 Desc;";

AnsiString SQLWhereText;
TVarRec args[1];
if(!AllTerminals)
{
SQLWhereText += " WHERE TERMINAL_NAME = :TERMINAL_NAME "
"AND TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore ";//
}
else
{
SQLWhereText += " WHERE TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore "; // Desc
}
args[0] = SQLWhereText;

AnsiString Result = Format.Format(Format,args,0);

IBInternalQuery->Close();
IBInternalQuery->SQL->Text = Result;
IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime =  Date;
IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = Date + 1; // Midnight Tomorrow.;
if(IBInternalQuery->Params->FindParam("TERMINAL_NAME"))
{
IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
}
IBInternalQuery->Open();
First();
}
catch(Exception &E)
{
TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
}
}
 */

void TManagerReceipt::Close()
{
}

// ---------------------------------------------------------------------------
bool TManagerReceipt::Get(Database::TDBTransaction &DBTransaction)
{
   bool retVal = false;
   try
   {
	  if (!Array.empty())
	  {
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		 AnsiString Format;
		 if (Array[ArrayIndex].first == 1)
		 {
			Format = "Select first(1) RECEIPT,INVOICE_NUMBER, TOTAL , SECURITY_REF From ARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY;";
		 }
		 else
		 {
			Format = "Select first(1) RECEIPT,INVOICE_NUMBER, TOTAL , SECURITY_REF From DAYARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY;";
		 }

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = Format;
		 IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Array[ArrayIndex].second;
		 IBInternalQuery->ExecQuery();

		 Receipt->Clear();
		 IBInternalQuery->FieldByName("RECEIPT")->SaveToStream(Receipt);
                 ReceiptValue = IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                 Sec_Ref= IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
                 InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
		 Receipt->Position = 0;
		 retVal = true;
	  }
	  else
	  {
		 Receipt->Clear();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return retVal;
}

void TManagerReceipt::SetCurrentDate(TDateTime SelectedDate)
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

bool TManagerReceipt::FindRef(AnsiString Reference)
{
   bool RetVal = false;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  AnsiString Format = "Select 1 TABLETYPE,ARCBILL_KEY From ARCREF %0:s " "Union All "
		  "Select 2 TABLETYPE,ARCBILL_KEY From DAYARCREF %0:s Order By 2 Desc;";
          //AnsiString SQLWhereText = "WHERE RECEIPT is not null AND REFERENCE like :REFERENCE";
	  AnsiString SQLWhereText = "WHERE REFERENCE like :REFERENCE";
	  TVarRec args[1];
	  args[0] = SQLWhereText;
	  AnsiString Result = Format.Format(Format, args, 0);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = Result;
	  IBInternalQuery->ParamByName("REFERENCE")->AsString = "%" + Reference + "%";
	  IBInternalQuery->ExecQuery();

	  Array.clear();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 Array.push_back(TableKeyPair(IBInternalQuery->FieldByName("TABLETYPE")->AsInteger,
			   IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger));
		 RetVal = true;
	  }
	  Get(DBTransaction);

	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

bool TManagerReceipt::FindMember(int ContactKey)
{
   bool RetVal = false;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  // Get the members contact key.

	  AnsiString Format = "Select 1 TABLETYPE, ARCBILL_KEY From ARCHIVE %0:s group by  ARCBILL_KEY " " Union All "
		  "Select 2 TABLETYPE,ARCBILL_KEY From  DAYARCHIVE %0:s group by  ARCBILL_KEY Order By 2 Desc;";

          //AnsiString SQLWhereText = "WHERE RECEIPT is not null AND LOYALTY_KEY = :LOYALTY_KEY";
	  AnsiString SQLWhereText = "WHERE LOYALTY_KEY = :LOYALTY_KEY";
	  TVarRec args[1];
	  args[0] = SQLWhereText;
	  AnsiString Result = Format.Format(Format, args, 0);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = Result;
	  IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  Array.clear();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 Array.push_back(TableKeyPair(IBInternalQuery->FieldByName("TABLETYPE")->AsInteger,
			   IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger));
		 RetVal = true;
	  }
	  Get(DBTransaction);

	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

bool TManagerReceipt::Find(AnsiString InvoiceNo)
{
   bool RetVal = false;
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
			Array.clear();
			Array.push_back(TableKeyPair(1, key));
			Get(DBTransaction);
			RetVal = true;
		 }
	  }
	  else
	  {
		 Array.clear();
		 Array.push_back(TableKeyPair(2, key));
		 Get(DBTransaction);
		 RetVal = true;
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

void TManagerReceipt::Get(TStringList *Lines)
{
   try
   {
   //   UnicodeString data2 = "" ;
      bool IsFirstOccurance = false;
	  ManagerReceipt->Receipt->Position = 0;
	  AnsiString TrimmedLine;
	  char *Line = (char*)ManagerReceipt->Receipt->Memory;
	  for (int j = 0; j < ManagerReceipt->Receipt->Size; j++)
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
            if((!IsFirstOccurance)  && ((int)Line[j]>32) &&(TGlobalSettings::Instance().ReprintReceiptLabel != "")
                && IsStartOfReceiptInfo(Lines) && TGlobalSettings::Instance().ReprintReceiptLabel.Trim().Length() != 0)
             {
                 InsertReprintLabel(Lines,IsFirstOccurance);
             }

			if (Line[j] == char(196))
			{
			   TrimmedLine += "-";
			}
			else
			{
               AnsiString check = "";
               if(Lines->Count == 4)
               {
                  check += Line[j];
               }
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
//------------------------------------------------------------------------------
bool TManagerReceipt::IsStartOfReceiptInfo(TStringList *Lines)
{
    bool retValue = true;
    for(int i = 0; i < Lines->Strings[Lines->Count -1].Length(); i++)
    {
        if(Lines->Strings[Lines->Count - 1].SubString(i,1) != '_' )
        {
            retValue = false;
            break;
        }
    }
    return retValue;
}
//------------------------------------------------------------------------------
void TManagerReceipt::InsertReprintLabel(TStringList *Lines,bool &IsFirstOccurance)
{
    IsFirstOccurance = true;
    AnsiString newLine = "";
    for(int i = 0; i < Lines->Strings[Lines->Count -1].Length(); i++)
    {
        newLine += " ";
    }
    Lines->Add(newLine);
    int RowCount =  Lines->Count;
    if(RowCount == 0)
      {
         Lines->Add(TGlobalSettings::Instance().ReprintReceiptLabel);
      }
     else
      {
         int InsertPosition =  (Lines->Strings[RowCount-1].Length() - TGlobalSettings::Instance().ReprintReceiptLabel.Length())/2;
         Lines->Strings[RowCount-1] = Lines->Strings[RowCount-1].SubString(TGlobalSettings::Instance().ReprintReceiptLabel.Length(),Lines->Strings[RowCount - 1].Length() - TGlobalSettings::Instance().ReprintReceiptLabel.Length());
         Lines->Strings[RowCount-1] = Lines->Strings[RowCount-1].Insert(TGlobalSettings::Instance().ReprintReceiptLabel,InsertPosition);
      }
}
//------------------------------------------------------------------------------

void TManagerReceipt::AddDuplicateLabel(TMemoryStream* ReceiptToEdit,TStringList *Lines)
{
   try
   {
      bool IsFirstOccurance = false;
	  ReceiptToEdit->Position = 0;
	  AnsiString TrimmedLine;
	  char *Line = (char*)ReceiptToEdit->Memory;
	  for (int j = 0; j < ReceiptToEdit->Size; j++)
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
            if((!IsFirstOccurance)  && ((int)Line[j]>32) &&(TGlobalSettings::Instance().ReprintReceiptLabel != "")
                && IsStartOfReceiptInfo(Lines))
             {
                 InsertReprintLabel(Lines,IsFirstOccurance);
             }
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

void TManagerReceipt::PrintLastReceipt()
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("There is no Receipt printer selected.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {
	  try
	  {
			Database::TDBTransaction DBTransaction(DBControl);
	  		DBTransaction.StartTransaction();
            bool proceed = true;
            GetLastReceipt(DBTransaction);
            if(TGlobalSettings::Instance().IsFiscalStorageEnabled)
            {
                proceed = CanReprintReceipt(DBTransaction,InvoiceNumber);
            }
            if(proceed)
            {
                Receipt->Position = 0;
                PrintDuplicateReceipt(Receipt);
                TMallExportUpdateAdaptor exportUpdateAdaptor;
                exportUpdateAdaptor.UpdateExportTableOnReprint(ReceiptValue, &DBTransaction);

                TDBSecurity::ProcessSecurityFill(DBTransaction, Sec_Ref, TDeviceRealTerminal::Instance().User.LatestLoginContactKey,
                SecurityTypes[secReprintReceipt], TDeviceRealTerminal::Instance().User.LatestLoginName,
                TDeviceRealTerminal::Instance().User.Initials, Now(),TDeviceRealTerminal::Instance().ID.Name,InvoiceNumber);
                if(TGlobalSettings::Instance().IsFiscalStorageEnabled)
                {
                    std::auto_ptr<TFiscalDataUtility> dataUtility(new TFiscalDataUtility());
                    AnsiString data = dataUtility->GetPOSPlusData(InvoiceNumber);
                    AnsiString response = TDeviceRealTerminal::Instance().FiscalPort->SetFiscalData(data, eFiscalCopyReceipt);
                }
            }
            DBTransaction.Commit();
	  }
	  __finally
	  {
		 //delete Printout;
	  }
   }
}

void TManagerReceipt::Print()
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("There is no Receipt printer selected.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
      try
      {
        bool proceed = true;
        if(TGlobalSettings::Instance().IsFiscalStorageEnabled)
        {
            proceed = CanReprintReceipt(DBTransaction,InvoiceNumber);
        }
        if(proceed)
        {
            Receipt->Position = 0;
            PrintDuplicateReceipt(Receipt,true);
            TMallExportUpdateAdaptor exportUpdateAdaptor;
            exportUpdateAdaptor.UpdateExportTableOnReprint(ReceiptValue, &DBTransaction);

            TDBSecurity::ProcessSecurityFill(DBTransaction, Sec_Ref, TDeviceRealTerminal::Instance().User.LatestLoginContactKey,
            SecurityTypes[secReprintReceipt], TDeviceRealTerminal::Instance().User.LatestLoginName,
            TDeviceRealTerminal::Instance().User.Initials, Now(),TDeviceRealTerminal::Instance().ID.Name,InvoiceNumber);
            if(TGlobalSettings::Instance().IsFiscalStorageEnabled)
            {
                std::auto_ptr<TFiscalDataUtility> dataUtility(new TFiscalDataUtility());
                AnsiString data = dataUtility->GetPOSPlusData(InvoiceNumber);
                AnsiString response = TDeviceRealTerminal::Instance().FiscalPort->SetFiscalData(data, eFiscalCopyReceipt);
            }
        }
      }
      __finally
      {
         //delete Printout;
      }
	  DBTransaction.Commit();
   }
}
//------------------------------------------------------------------------------
bool TManagerReceipt::CanReprintReceipt(Database::TDBTransaction &DBTransaction, AnsiString InvoiceNumber)
{
    bool retValue = true;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT SECURITY_KEY FROM SECURITY a WHERE "
                                 "a.NOTE = :NOTE AND a.SECURITY_EVENT = :SECURITY_EVENT ";
    IBInternalQuery->ParamByName("NOTE")->AsString = InvoiceNumber;
    IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = "Reprint Receipt";
    IBInternalQuery->ExecQuery();
    int i = 0;
    for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
    {
       i++;
       if(i >= 1)
       {
            retValue = false;
            MessageBox("This Receipt has been printed more than once.\n"
                       "This operation can be performed only once when PosPlus is enabled",
                       "Warning", MB_OK + MB_ICONWARNING);
            break;
       }
    }
    return retValue;
}
//------------------------------------------------------------------------------
void TManagerReceipt::PrintDuplicateReceipt(TMemoryStream* DuplicateReceipt,bool ReprintReceiptWithCompanydetails)
{
    try
    {
		std::auto_ptr <TStringList> StringReceipt(new TStringList);
		Get(StringReceipt.get());

		TReqPrintJob* TempReceipt = new TReqPrintJob(&TDeviceRealTerminal::Instance());
		TempReceipt->JobType = pjReceiptReceipt;
		TPrintout *Printout1 = new TPrintout;
		Printout1->Printer = TComms::Instance().ReceiptPrinter;
		TempReceipt->Printouts->Add(Printout1);
        // to do add condition
		UnicodeString data = "Sold to:" ;
                // end of condition
		for(int i = 0; i < StringReceipt->Count; i++)
		{
		   Printout1->PrintFormat->Line->ColCount = 1;
		   if(StringReceipt->Strings[i].Trim() != TGlobalSettings::Instance().ReprintReceiptLabel.Trim())
		   {
				Printout1->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		   }
		   else
		   {
			  if(TGlobalSettings::Instance().IsFiscalStorageEnabled)
				Printout1->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
			  else
				Printout1->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		   }
		   Printout1->PrintFormat->Line->Columns[0]->Width = Printout1->PrintFormat->Width;
		   Printout1->PrintFormat->Line->Columns[0]->Text = StringReceipt->Strings[i];

		   Printout1->PrintFormat->AddLine();
		}
        // to do :- add condition
		Printout1->PrintFormat->Line->Columns[0]->Text = data;
		Printout1->PrintFormat->AddLine();

		UnicodeString data2 = TGlobalSettings::Instance().Companydetails;
		int widthprinter = Printout1->PrintFormat->Width ;
		int datacount = data2.Length();
		UnicodeString tempData = "";
		UnicodeString truncatedData = "";
		int dataAdded = 0;
		truncatedData =  data2;
		for(int i = 0; i < data2.Length();)
		{
			if(truncatedData.Pos("\n"))
			{
				tempData = truncatedData.SubString(0,truncatedData.Pos("\n"));
			}
			else
			{
				tempData = truncatedData;
			}
			if(tempData.Length() >= widthprinter)
			{
				int storedValue = 0;
				for(int j = 0; j < tempData.Length();)
				{
					Printout1->PrintFormat->Line->Columns[0]->Text = tempData.SubString(storedValue+1,widthprinter);
					Printout1->PrintFormat->AddLine();
					dataAdded += tempData.SubString(storedValue+1,widthprinter).Length();
					storedValue += widthprinter;
					j += storedValue;
				}
			}
			else
			{
				bool skipAdding = false;
				if(tempData.Length() == 1 && (tempData.Pos("\n") || tempData.Pos("\r")))
				   skipAdding = true;
				if(!skipAdding)
				{
					Printout1->PrintFormat->Line->Columns[0]->Text = tempData;
					Printout1->PrintFormat->AddLine();
				}
				dataAdded += tempData.Length();
			}
			truncatedData = data2.SubString(dataAdded,data2.Length()-dataAdded);
			i = dataAdded;
		}
        // end of condition
		Printout1->PrintFormat->PartialCut();

		TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
    }
    catch(Exception &Exc)
    {
        MessageBox(Exc.Message,"Exception",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}

bool TManagerReceipt::CanApplyTipOnThisReceiptsTransaction(WideString &outPaymentRefNumber,Currency &outOriginalVisaPaymentAmount,int &outArcbillKey)
{
    bool retVal = false;

    try
    {
	  if (!Array.empty())
	  {
		 std::vector<AnsiString> tippableCardTypes = TDeviceRealTerminal::Instance().PaymentSystem->GetTippableCardTypes();
		 AnsiString tipCardTypesStr = "";

		 for(std::vector<AnsiString>::iterator i = tippableCardTypes.begin() ; i != tippableCardTypes.end() ; i++)
		 {
			 tipCardTypesStr += "\'";
			 tipCardTypesStr += *i;
			 tipCardTypesStr += "\',";
		 }

		 tipCardTypesStr = tipCardTypesStr.SubString(0, tipCardTypesStr.Length()-1);

         if( tipCardTypesStr.Length() > 0 )
         {
             Database::TDBTransaction DBTransaction(DBControl);
             DBTransaction.StartTransaction();

             TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
             IBInternalQuery->Close();
             IBInternalQuery->SQL->Text = "Select DAP.PAY_TYPE_DETAILS, DAP.SUBTOTAL, DAP.DAYARCBILLPAY_KEY, DAP.ARCBILL_KEY, DAP.PROPERTIES"
                                            " from DAYARCBILLPAY DAP "
                                            " where DAP.ARCBILL_KEY = :ARCBILL_KEY "
                                            " and "
                                            " CHARACTER_LENGTH(TRIM(BOTH ' ' from DAP.PAY_TYPE_DETAILS)) > 0 "
                                            " and TRIM(DAP.PAYMENT_CARD_TYPE) in ( " + tipCardTypesStr + ") "
                                            " AND PAY_TYPE not in ('Cash','Tip') ";
             IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Array[ArrayIndex].second;

             IBInternalQuery->ExecQuery();

             if(!IBInternalQuery->Eof)
             {
                outPaymentRefNumber = WideString( IBInternalQuery->Fields[0]->AsString );
                outOriginalVisaPaymentAmount = IBInternalQuery->Fields[1]->AsDouble;
                outArcbillKey = Array[ArrayIndex].second;
                AnsiString properties = IBInternalQuery->Fields[4]->AsString;
                AnsiString proptoSearch = IntToStr(ePayTypeAllowTips) + ",";
                if(TStringTools::Instance()->HasAllProperties(properties,proptoSearch))
                    retVal = true;
             }

             DBTransaction.Commit();
          }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }

   return retVal;
}

