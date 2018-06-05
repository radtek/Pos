// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "SelectZed.h"
#include "Comms.h"
#include "PrintFormat.h"
#include "MMMessageBox.h"
#include "printout.h"
#include "SendEmail.h" // 4104

#define RECEIPT_DELIMITER "#####"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"


// ---------------------------------------------------------------------------
__fastcall TfrmSelectZed::TfrmSelectZed(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl)
{

}

__fastcall TfrmSelectZed::~TfrmSelectZed()
{
   delete Zed;
}
//----------------------------------------------------------------------------
void TfrmSelectZed::Initialize(ZEDMode inMode)
{
   SelectZedMode = inMode;
   Zed = new TCurrentZed(DBControl,SelectZedMode);
   Zed->Last();
   CurrentDate = Date();
   calZed->Date = CurrentDate;
}
// ---------------------------------------------------------------------------
void TfrmSelectZed::Execute()
{
   if (!ReceiptsExist())
   {
	  MessageBox("There are no Z reports to print. To view old Z reports, view the files in the 'ZDir' Directory ", "Warning",
		 MB_ICONWARNING + MB_OK);
	  return;
   }
   else
   {
	  CurrentDate = Date();
	  ShowModal();
   }
}
// ---------------------------------------------------------------------------
void TfrmSelectZed::ShowReceipt()
{
   std::auto_ptr <TStringList> Lines(new TStringList);
   Zed->CurrentZed->Position = 0;
   AnsiString TrimmedLine;
   char *Line = (char*)Zed->CurrentZed->Memory;
   for (int j = 0; j < Zed->CurrentZed->Size; j++)
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
		 else if (Line[j] == char(205))
		 {
			TrimmedLine += "=";
		 }
		 else
		 {
			TrimmedLine += Line[j];
		 }
	  }
   }
   if (TrimmedLine != "")
	  Lines->Add(TrimmedLine);

   memReceipt->Lines->Assign(Lines.get());
}
// ---------------------------------------------------------------------------
bool TfrmSelectZed::ReceiptsExist()
{
   bool ReceiptsExist = false;
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) FROM ZEDS WHERE TERMINAL_NAME = :TERMINAL_NAME";
   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
   IBInternalQuery->ExecQuery();
   if (!IBInternalQuery->Fields[0]->IsNull)
   {
      ReceiptsExist = true;
   }
   DBTransaction.Commit();
   return ReceiptsExist;
}
//-----------------------------------------------------------------------------
void __fastcall TfrmSelectZed::btnPrintClick(TObject *Sender)
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("Please select a receipt printer from Setup first.", "Print error", MB_OK + MB_ICONERROR);
   }
   else
   {
	  TPrintout *Printout = new TPrintout;
	  Printout->Printer = TComms::Instance().ReceiptPrinter;
	  try
	  {
		 Zed->CurrentZed->Position = 0;
		 Printout->PrintToPrinterStream(Zed->CurrentZed, TComms::Instance().ReceiptPrinter.UNCName());
	  }
	  __finally
	  {
		 delete Printout;
	  }
   }
}

// ---------------------------------------------------------------------------
void TfrmSelectZed::PrintReceipt(TStringList *Zed)
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	  throw Exception("No receipt printer selected.");
   TPrintout *Printout = new TPrintout;
   try
   {
	  Printout->PrintToPrinterRaw(Zed, TComms::Instance().ReceiptPrinter.UNCName());
   }
   __finally
   {
	  delete Printout;
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::btnNextClick(TObject *Sender)
{

   bool Eof = Zed->Next();
   ShowReceipt();
   if (!Eof)
   {
	  btnNext->Enabled = true;
	  btnNext->Color = clNavy;

	  btnPrev->Enabled = true;
	  btnPrev->Color = clNavy;
   }
   else
   {
	  if (Zed->Count() > 1)
	  {
		 btnPrev->Enabled = true;
		 btnPrev->Color = clNavy;
	  }

	  btnNext->Enabled = false;
	  btnNext->Color = clMaroon;
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::btnPrevClick(TObject *Sender)
{
   bool Bof = Zed->Prior();
   ShowReceipt();
   if (!Bof)
   {
	  btnPrev->Enabled = true;
	  btnPrev->Color = clNavy;

	  btnNext->Enabled = true;
	  btnNext->Color = clNavy;
   }
   else
   {
	  if (Zed->Count() > 1)
	  {
		 btnNext->Enabled = true;
		 btnNext->Color = clNavy;
	  }

	  btnPrev->Enabled = false;
	  btnPrev->Color = clMaroon;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::btnCloseClick(TObject *Sender)
{
   Close();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::calZedClick(TObject *Sender)
{
   CurrentDate = calZed->Date;
}
// ---------------------------------------------------------------------------
void TfrmSelectZed::SetCurrentDate(TDateTime SelectedDate)
{
   FSelectedDate = SelectedDate;
   Zed->TimeFilter = SelectedDate;
   bool FoundZeds = Zed->Last();
   ShowReceipt();

   btnNext->Enabled = false;
   btnNext->Color = clMaroon;

   if (FoundZeds)
   {
	  btnPrev->Enabled = true;
	  btnPrev->Color = clNavy;
	  ShowReceipt();
   }
   else
   {
	  btnPrev->Enabled = false;
	  btnPrev->Color = clMaroon;
   }
}
//-----------------------------------------------------------------------------
void __fastcall TfrmSelectZed::sbAllClick(TObject *Sender)
{
   Zed->TerminalFilter = sbThisTerminal->Down;
   bool FoundZeds = Zed->Last();
   ShowReceipt();

   btnNext->Enabled = false;
   btnNext->Color = clMaroon;

   if (FoundZeds)
   {
	  btnPrev->Enabled = true;
	  btnPrev->Color = clNavy;
   }
   else
   {
	  btnPrev->Enabled = false;
	  btnPrev->Color = clMaroon;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::btnBillDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectZed::btnBillUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
// ---------------------------------------------------------------------------
__fastcall TCurrentZed::TCurrentZed(Database::TDBControl &inDBControl, ZEDMode inMode) : DBControl(inDBControl)
{
   ZedMode = inMode;
   CurrentZed = new TMemoryStream;
   TimeFilter = Date();
   TerminalFilter = true;
   Last();
}
// ---------------------------------------------------------------------------
__fastcall TCurrentZed::~TCurrentZed()
{
   delete CurrentZed;
}
//-----------------------------------------------------------------------------
AnsiString  terminalName="";  //MM-4104
TDateTime date_time;
//-----------------------------------------------------------------------------
bool TCurrentZed::Get()
{
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
      if(ZedMode == ePOSZED)
        GetPOSZed(DBTransaction);
      else
        GetEFTPOSZed(DBTransaction);
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return true;
}
//-----------------------------------------------------------------------------
bool TCurrentZed::GetPOSZed(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT REPORT, TERMINAL_NAME, TIME_STAMP FROM ZEDS WHERE Z_KEY = :Z_KEY";
    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
    IBInternalQuery->ExecQuery();

    CurrentZed->Clear();
    IBInternalQuery->FieldByName("REPORT")->SaveToStream(CurrentZed);
    terminalName = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
    date_time = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;

    CurrentZed->Position = 0;
    return true;
}
//-----------------------------------------------------------------------------
bool TCurrentZed::GetEFTPOSZed(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT ZED_RECEIPT, POS_TERMINALNAME, TIME_STAMP FROM EFTPOSZED WHERE EFTPOS_ZED_ID = :EFTPOS_ZED_ID";
    IBInternalQuery->ParamByName("EFTPOS_ZED_ID")->AsInteger = ZedKey;
    IBInternalQuery->ExecQuery();

    CurrentZed->Clear();
    IBInternalQuery->FieldByName("ZED_RECEIPT")->SaveToStream(CurrentZed);
    terminalName = IBInternalQuery->FieldByName("POS_TERMINALNAME")->AsString;
    date_time = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;

    CurrentZed->Position = 0;
    return true;
}
//-----------------------------------------------------------------------------
bool TCurrentZed::Next()
{
//   MessageBox("TCurrentZed::Next()","",MB_OK);
   int MaxKey = Max();
   if (ZedKey + 1 <= MaxKey)
   {
	  ZedKey++;
   }
   Get();
   return ZedKey == MaxKey;
}
//-----------------------------------------------------------------------------
int TCurrentZed::Count()
{
   int MaxKey = Max();
   int MinKey = Min();
   return(MaxKey - MinKey) + 1;
}
//-----------------------------------------------------------------------------
bool TCurrentZed::Prior()
{
//   MessageBox("TCurrentZed::Prior()","",MB_OK);
   int MinKey = Min();
   if (ZedKey - 1 >= MinKey)
   {
	  ZedKey--;
   }
   Get();
   return ZedKey == MinKey;
}
//-----------------------------------------------------------------------------
bool TCurrentZed::Last()
{
   ZedKey = Max();
   Get();
   return(ZedKey != -1);
}
//-----------------------------------------------------------------------------
bool TCurrentZed::First()
{
   ZedKey = Min();
   Get();
   return(ZedKey != -1);
}
//-----------------------------------------------------------------------------
int TCurrentZed::Max()
{
   int RetVal = -1;

   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   try
   {
        if(ZedMode == ePOSZED)
            RetVal = GetPOSMax(DBTransaction);
        else
            RetVal = GetEFTPOSMax(DBTransaction);
        DBTransaction.Commit();
   }
   catch(Exception &Ex)
   {
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,Ex.Message);
        DBTransaction.Rollback();
   }
   return RetVal;
}
//-----------------------------------------------------------------------------
int TCurrentZed::GetPOSMax(Database::TDBTransaction &DBTransaction)
{
    int RetVal = -1;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   AnsiString SQLWhereText;
   if (TerminalFilter)
   {
	  SQLWhereText += " WHERE TERMINAL_NAME = :TERMINAL_NAME " "AND TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
   }
   else
   {
	  SQLWhereText += " WHERE TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
   }

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) FROM ZEDS" + SQLWhereText;
   if (TerminalFilter)
   {
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;

   }
   IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime = TimeFilter;
   IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = TimeFilter + 1;
   IBInternalQuery->ExecQuery();
   if (!IBInternalQuery->Fields[0]->IsNull)
   {
	  RetVal = IBInternalQuery->Fields[0]->AsInteger;
   }
   return RetVal;
}
//-----------------------------------------------------------------------------
int TCurrentZed::GetEFTPOSMax(Database::TDBTransaction &DBTransaction)
{
    int RetVal = -1;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    AnsiString SQLWhereText;
    if (TerminalFilter)
    {
      SQLWhereText += " WHERE POS_TERMINALNAME = :POS_TERMINALNAME " "AND TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
    }
    else
    {
      SQLWhereText += " WHERE TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
    }

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT MAX(EFTPOS_ZED_ID) FROM EFTPOSZED" + SQLWhereText;
    if (TerminalFilter)
    {
      IBInternalQuery->ParamByName("POS_TERMINALNAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;

    }
    IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime = TimeFilter;
    IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = TimeFilter + 1;
    IBInternalQuery->ExecQuery();
    if (!IBInternalQuery->Fields[0]->IsNull)
    {
      RetVal = IBInternalQuery->Fields[0]->AsInteger;
    }
    return RetVal;
}
//-----------------------------------------------------------------------------
int TCurrentZed::Min()
{
    int RetVal = -1;

    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    try
    {
        if(ZedMode == ePOSZED)
            RetVal = GetPOSMin(DBTransaction);
        else
            RetVal = GetEFTPOSMin(DBTransaction);
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        MessageBox(Ex.Message,"Exception in Min()",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,Ex.Message);
        DBTransaction.Rollback();
    }
    return RetVal;
}
//------------------------------------------------------------------------------
int TCurrentZed::GetPOSMin(Database::TDBTransaction &DBTransaction)
{
    int RetVal = -1;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    AnsiString SQLWhereText;
    if (TerminalFilter)
    {
      SQLWhereText += " WHERE TERMINAL_NAME = :TERMINAL_NAME " "AND TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
    }
    else
    {
      SQLWhereText += " WHERE TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
    }

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT MIN(Z_KEY) FROM ZEDS" + SQLWhereText;
    if (TerminalFilter)
    {
      IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
    }
    IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime = TimeFilter;
    IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = TimeFilter + 1;
    IBInternalQuery->ExecQuery();
    if (!IBInternalQuery->Fields[0]->IsNull)
    {
      RetVal = IBInternalQuery->Fields[0]->AsInteger;
    }
    return RetVal;
}
//------------------------------------------------------------------------------
int TCurrentZed::GetEFTPOSMin(Database::TDBTransaction &DBTransaction)
{
    int RetVal = -1;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    AnsiString SQLWhereText;
    if (TerminalFilter)
    {
      SQLWhereText += " WHERE POS_TERMINALNAME = :POS_TERMINALNAME " "AND TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
    }
    else
    {
      SQLWhereText += " WHERE TIME_STAMP > :TimeStampAfter AND TIME_STAMP < :TimeStampBefore;";
    }

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT MIN(EFTPOS_ZED_ID) FROM EFTPOSZED" + SQLWhereText;
    if (TerminalFilter)
    {
      IBInternalQuery->ParamByName("POS_TERMINALNAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
    }
    IBInternalQuery->ParamByName("TimeStampAfter")->AsDateTime = TimeFilter;
    IBInternalQuery->ParamByName("TimeStampBefore")->AsDateTime = TimeFilter + 1;
    IBInternalQuery->ExecQuery();
    if (!IBInternalQuery->Fields[0]->IsNull)
    {
      RetVal = IBInternalQuery->Fields[0]->AsInteger;
    }
    return RetVal;
}
//------------------------------------------------------------------------------
void __fastcall TfrmSelectZed::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height < 1.4)
          {
	  	ScaleBy(Screen->Width, Temp);
          }
   }
   ClientWidth = Panel1->Left + Panel1->Width + Panel1->Left;
   ClientHeight = Panel1->Height + (Panel1->Top * 2);
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::FormShow(TObject *Sender)
{
    FormResize(this);
   	if(TGlobalSettings::Instance().EnableDontClearZedData || SelectZedMode == eEFTPOSZED)
	{
        btnSendEmail->Visible = false;
	}
   FormResize(this);
   EnableEmail = false;   //MM-4104
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectZed::btnSendEmailClick(TObject *Sender)
{

    UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;
    UnicodeString Dir = ExtractFilePath(Application->ExeName) + "ReprintZDIR";
    if (!DirectoryExists(Dir))
    {
       CreateDir(Dir);
    }

    UnicodeString filename = Dir + "\\" + "ZReport" + "-" + date_time.FormatString("yyyy-mm-dd - hh-mm-ss") +".txt";

    FormattedZed(Zed->CurrentZed)->SaveToFile(filename);
    AnsiString emailIds = TGlobalSettings::Instance().SaveEmailId;
    EmailSettingCheck();
    if(EnableEmail)
    {
      std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
      Processing->Message = "Sending Emails...";
      Processing->Show();
      SendEmail::Send(filename, "Zed Report", emailIds, "");
      Zed->CurrentZed->Clear();
    }
}
//-----------------------------------------------------------------------------
void __fastcall TfrmSelectZed::EmailSettingCheck()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text ="SELECT a.PROFILE_KEY, a.INTEGER_VAL FROM VARSPROFILE a "
	"left join PROFILE on PROFILE.PROFILE_KEY=a.PROFILE_KEY "
	" where a.VARIABLES_KEY=4099 and PROFILE.NAME=:NAME "  ;
	IBInternalQuery->ParamByName("NAME")->AsString =terminalName;
	IBInternalQuery->ExecQuery();
	int  TEmail = IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger;
	DBTransaction.Commit();

	if(TEmail > 0)
	{
      EnableEmail = true;
    }
	else
    {
	  EnableEmail = false;
    }
	//::::::::::::::::::::::::::::::::::::::::::::::::

}
//-----------------------------------------------------------------------------
TMemoryStream* TfrmSelectZed::FormattedZed(TMemoryStream *ZedToArchive)
	{

		TMemoryStream* tempStream = new TMemoryStream;
		ZedToArchive->Position = 0;
		int prevStreamArrVal = 0;
		for (unsigned int i=0;i<ZedToArchive->Size;i++)
		{
			char c;
			ZedToArchive->Read(&c, 1);
			int StreamArrDecimalval= int(c);
		 if(StreamArrDecimalval > 0 && StreamArrDecimalval != 27 && StreamArrDecimalval != 8 && StreamArrDecimalval != 16 && StreamArrDecimalval != 24 && StreamArrDecimalval != 30 && StreamArrDecimalval < 127)
        	{


              	if(StreamArrDecimalval==33&&prevStreamArrVal==114)
				{
					tempStream->Position--;
				}
				else
				{
					tempStream->Write(&c, 1);

				}
				prevStreamArrVal =StreamArrDecimalval;


		}
       }
        return tempStream;


     }
//-----------------------------------------------------------------------------
