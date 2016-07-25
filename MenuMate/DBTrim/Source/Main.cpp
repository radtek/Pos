//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "MMRegistry.h"
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <DateUtils.hpp>

#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
   : TForm(Owner)
{
  	Application->OnMessage = AppMessage;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
   if(DBTrimUtil)
   {
      DBTrimUtil->Cancel();
      delete DBTrimUtil;
      DBTrimUtil = NULL;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::AppMessage(tagMSG &Msg, bool &Handled)
{
   if(Msg.message == STATUSCHANGED)
   {
      if(Msg.lParam == STATUSBEGIN)
      {
         StatusBar1->Panels->Items[0]->Text = "Total Number of Records : " + IntToStr(DBTrimUtil->MaxCount);
         Timer->Enabled = true;
      }

      if(Msg.lParam == STATUSWORK)
      {
      }

      if(Msg.lParam == STATUSERROR)
      {
         MessageBox(this->Handle,DBTrimUtil->LastError.c_str(), "DB Error!!", MB_OK + MB_ICONERROR);
      }

      if(Msg.lParam == STATUSCOMPLETE)
      {
         Timer->Enabled = false;
         DBTrimUtil->Cancel();
         delete DBTrimUtil;
         DBTrimUtil = NULL;
         TouchBtn2->Caption = "Trim DB";
         MessageBox(this->Handle,"DB Trim Completed.", "Finished!!", MB_OK + MB_ICONERROR);
      }
   }
}


void TfrmMain::DrawCalendar()
{
   //TODO: Add your source code here
   tgYear->RowCount = 0;
   tgYear->ColCount = 10;
   tgYear->RowCount = 1;
   int TheCurrentYear = CurrentYear();
   for (int i = tgYear->ColCount-1; i >= 0 ; i--)
   {
      tgYear->Buttons[0][i]->Caption = IntToStr(TheCurrentYear);
      tgYear->Buttons[0][i]->Tag = TheCurrentYear--;
   }
   tgYear->Buttons[0][tgYear->ColCount - 2]->Latched = true;

   tgMonth->RowCount = 0;
   tgMonth->ColCount = 6;
   tgMonth->RowCount = 2;
   for (int i = 0; i < 2 ; i++)
   {
      for (int j = 0; j < 6 ; j++)
      {
         int CurrentMonth = ((i * tgMonth->ColCount )+j)+1;
         int CurrentMonthIndex = ((i * tgMonth->ColCount )+j);
         tgMonth->Buttons[i][j]->Tag = CurrentMonth;
         tgMonth->Buttons[i][j]->Caption = ShortMonthNames[CurrentMonthIndex];
         if(tgMonth->Buttons[i][j]->Tag == 3) // Latch March.
         {
            tgMonth->Buttons[i][j]->Latched = true;
            // Draw in the days.
            tgMonthMouseClick(NULL,TMouseButton() << mbLeft, TShiftState(), tgMonth->Buttons[i][j]);
         }
      }
   }
}
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
   FormResize(Sender);
   DrawCalendar();

   // Connect to the DB
	UnicodeString DatabasePath = "";
   RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

   UnicodeString InterbaseIP = "";
   RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);
   Label1->Caption = "DB To be Trimmed is :" + InterbaseIP + ":" + DatabasePath;


   TDateTime RetVal = Now();

  	Database::TDBControl DBControl;
   DBControl.Disconnect();
   DBControl.Init(Database::TDBSettings(InterbaseIP,DatabasePath,false));
   DBControl.Connect();

   // Select the Archives to be trashed.
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT MIN(TIME_STAMP)TIME_STAMP FROM ARCBILL";
   IBInternalQuery->ExecQuery();
   RetVal = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
   DBTransaction.Commit();
   DBControl.Disconnect();

   StatusBar1->Panels->Items[0]->Text = "Oldest Record : " + RetVal.DateString();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TouchBtn1MouseClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tgMonthMouseClick(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{

   // Retrive the Year and the Month;
   int Year, Month;
   for (int i = 0; i < tgYear->ColCount ; i++)
   {
      if(tgYear->Buttons[0][i]->Latched)
      {
         Year = tgYear->Buttons[0][i]->Tag;
      }
   }

   for (int i = 0; i < 2 ; i++)
   {
      for (int j = 0; j < 6 ; j++)
      {
         if(tgMonth->Buttons[i][j]->Latched)
         {
            Month = tgMonth->Buttons[i][j]->Tag;
         }
      }
   }

   // Draw up the Days.
   int TheDaysInAMonth = DaysInAMonth(Year, Month);

	tgDay->RowCount = 0;
   tgDay->ColCount = 7;
	tgDay->RowCount = 5;

   for (int i = 0; i < tgDay->RowCount ; i++)
   {
      for (int j = 0; j < tgDay->ColCount; j++)
      {
         int CurrentDay = (i * tgDay->ColCount )+j +1;
         tgDay->Buttons[i][j]->Caption = IntToStr(CurrentDay);
         tgDay->Buttons[i][j]->Tag = CurrentDay;
			tgDay->Buttons[i][j]->Visible = true;

         if(tgDay->Buttons[i][j]->Tag == TheDaysInAMonth)
         {
            tgDay->Buttons[i][j]->Latched = true;
         }
      }
   }

   for (int k = 0; k < tgDay->RowCount ; k++)
   {
      for (int j = 0; j < tgDay->ColCount; j++)
      {
         if(tgDay->Buttons[k][j]->Tag > TheDaysInAMonth)
         {
             tgDay->Buttons[k][j]->Visible = false;
         }
      }
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TouchBtn2MouseClick(TObject *Sender)
{
   if(DBTrimUtil)
   {
      if (MessageBox(this->Handle,"A DB Trim is in progress do you whish to cancel it?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
      {
         DBTrimUtil->Cancel();
         DBTrimUtil = NULL;
         MessageBox(this->Handle,"DB Trim Cancelled.", "Human Error!!", MB_OK + MB_ICONERROR);
      }
   }
   else
   {
      // Retrive the Year and the Month;
      int Year, Month, Day;
      for (int i = 0; i < tgYear->ColCount ; i++)
      {
         if(tgYear->Buttons[0][i]->Latched)
         {
            Year = tgYear->Buttons[0][i]->Tag;
         }
      }

      for (int i = 0; i < 2 ; i++)
      {
         for (int j = 0; j < 6 ; j++)
         {
            if(tgMonth->Buttons[i][j]->Latched)
            {
               Month = tgMonth->Buttons[i][j]->Tag;
            }
         }
      }

      for (int i = 0; i < tgDay->RowCount ; i++)
      {
         for (int j = 0; j < tgDay->ColCount; j++)
         {
            if(tgDay->Buttons[i][j]->Latched)
            {
               Day = tgDay->Buttons[i][j]->Tag;
            }
         }
      }

      TDateTime BoomTime = EncodeDate(Year, Month, Day);

      AnsiString Warning = "Warning proceeding will erase all historic sales information "
      " prior to " + DateToStr(BoomTime) + " are you certian you wish to proceed? Remeber to back up your DB.";

      if(BoomTime > Now())
      {
         MessageBox(this->Handle,"You cant trim information that hasnt happened yet.", "Human Error!!", MB_OK + MB_ICONERROR);
      }
      else if (MessageBox(this->Handle,Warning.c_str(), "Warning, Ensure you have backed up before proceeding.", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
      {
         if (MessageBox(this->Handle,Warning.c_str(), "Last Warning!!, Really ensure you have backed up before proceeding.", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
         {
            DBTrimUtil = new TDBTrimUtil;
            DBTrimUtil->Start(BoomTime);
            TouchBtn2->Caption = "Stop Trim DB";
         }
      }
   }
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   if(DBTrimUtil)
   {
      CanClose = false;
      if (MessageBox(this->Handle,"A DB Trim is in progress do you wish to cancel it?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
      {
         DBTrimUtil->Cancel();
         DBTrimUtil = NULL;
         CanClose = true;
      }
   }
   else
   {
      CanClose = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::StatusBar1DrawPanel(TStatusBar *StatusBar,
      TStatusPanel *Panel, const TRect &Rect)
{
	TCanvas *pCanvas = StatusBar1->Canvas;
   TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;
   pCanvas->Font->Style = TFontStyles() << fsBold;
	pCanvas->Font->Color = clWhite;
	pCanvas->Brush->Color = clNavy;
	pCanvas->Brush->Style = bsSolid;

   double Scaler = 0;
   if(DBTrimUtil)
   {
      if(DBTrimUtil->MaxCount)
      Scaler = double(DBTrimUtil->Position) / double(DBTrimUtil->MaxCount);
   }

	pCanvas->FillRect( TRect(Rect.left, Rect.top, Rect.left + int((Rect.right - Rect.left) * Scaler), Rect.bottom) ); // clear the rectangle

	AnsiString Caption = StatusBar1->Panels->Items[1]->Text;

	int TextHeight = DrawTextEx(pCanvas->Handle,
                               Caption.c_str(),
                               Caption.Length(),
										 &TRect(Rect.left, Rect.top, Rect.right, Rect.bottom),
                               DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX,
                               NULL) + 4;

   TRect CentredRect = Rect;
   int RowHeight = Rect.Bottom - Rect.Top;
	if (RowHeight > TextHeight)
   {
   	CentredRect.Top += (RowHeight - TextHeight) / 2;
		CentredRect.Bottom = CentredRect.Top + TextHeight;
   }
	DrawTextEx(pCanvas->Handle, Caption.c_str(), Caption.Length(), &CentredRect, DT_CENTER|DT_WORDBREAK|DT_NOPREFIX, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TimerTimer(TObject *Sender)
{
   if(DBTrimUtil)
   {
      StatusBar1->Panels->Items[1]->Text = IntToStr(DBTrimUtil->Position); " / " + IntToStr(DBTrimUtil->MaxCount);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TouchBtn3MouseClick(TObject *Sender)
{
//	std::auto_ptr<TfrmReportDisplay> frmReportDisplay(TfrmReportDisplay::Create<TfrmReportDisplay>(this));
	AnsiString ReportLocation = ExtractFilePath(Application->ExeName) + "\\Help\\DB_Trim\\DB_Trim_Help_Index.html";
   ShellExecute(this->Handle,"open",
                ReportLocation.c_str(),
                NULL,
                AnsiString(ExtractFilePath(Application->ExeName) + "\\Help\\DB_Trim\\").c_str(),
                SW_SHOWMAXIMIZED);

//	frmReportDisplay->Navigate(ReportLocation);
//	frmReportDisplay->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	if (frmMain->Tag != Screen->Width)
   {
      int Temp = frmMain->Tag;
		frmMain->Tag = Screen->Width;

      frmMain->ScaleBy(Screen->Width, Temp);

      frmMain->Width = Screen->Width;
      frmMain->Height = Screen->Height;
   }
}
//---------------------------------------------------------------------------

