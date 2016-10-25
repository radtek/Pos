//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Synd.h"
#include "MMTouchNumpad.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IBSQL"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	IBDatabase.Disconnect();
	IBDatabase.Init(Database::TDBSettings("",Edit2->Text,true));
	IBDatabase.Connect();

    ComboBox1->Items->Clear();

	Database::TDBTransaction DBTransaction(IBDatabase);
  	DBTransaction.StartTransaction();
   	ManagerSyndicateCode.Initialise(DBTransaction);
   	ManagerSyndicateCode.First(false);
  	DBTransaction.Commit();
	for (ManagerSyndicateCode.First(false);!ManagerSyndicateCode.Eof();ManagerSyndicateCode.Next())
    {
    	ComboBox1->Items->AddObject(ManagerSyndicateCode.SyndCode().Name,(TObject *)ManagerSyndicateCode.SyndCode().SyndCodeKey);
   	}
	Application->MessageBox(UnicodeString("Connected").w_str(),UnicodeString("Information").w_str(),MB_OK + MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    if(ComboBox1->ItemIndex >= 0)
    {
       Database::TDBTransaction DBTransaction(IBDatabase);
       DBTransaction.StartTransaction();
       ManagerSyndicateCode.ExportToFile(DBTransaction,(int)ComboBox1->Items->Objects[ComboBox1->ItemIndex], Edit1->Text);
       DBTransaction.Commit();
		Application->MessageBox(UnicodeString("Saved").w_str(),UnicodeString("Information").w_str(),MB_OK + MB_ICONINFORMATION);
    }
    else
    {
       Application->MessageBox(UnicodeString("Pick a Synd Code").w_str(),UnicodeString("Information").w_str(),MB_OK + MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
    if(ComboBox1->ItemIndex >= 0)
    {
       Database::TDBTransaction DBTransaction(IBDatabase);
       DBTransaction.StartTransaction();
       ManagerSyndicateCode.ImportFromFile(DBTransaction,(int)ComboBox1->Items->Objects[ComboBox1->ItemIndex], Edit1->Text);
       DBTransaction.Commit();
       Application->MessageBox(UnicodeString("Imported").w_str(),UnicodeString("Information").w_str(),MB_OK + MB_ICONINFORMATION);
    }
    else
    {
       Application->MessageBox(UnicodeString("Pick a Synd Code").w_str(),UnicodeString("Information").w_str(),MB_OK + MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnDisplayClick(TObject *Sender)
{
	if(!ManagerSyndicateCode.Empty())
    {
      AnsiString PIN = "";
      std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
      frmTouchNumpad->Caption = "Enter Access PIN";
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Mode = pmPIN;
      if (frmTouchNumpad->ShowModal() == mrOk)
      {
         PIN = frmTouchNumpad->STRResult;
      }

      if (PIN == "7410")
      {
         for (ManagerSyndicateCode.First(false);!ManagerSyndicateCode.Eof();ManagerSyndicateCode.Next())
         {
            Application->MessageBox(UnicodeString(ManagerSyndicateCode.SyndCode().DecryptedSyndCode).w_str(),UnicodeString(ManagerSyndicateCode.SyndCode().Name).w_str(),MB_OK + MB_ICONINFORMATION);
         }
      }
      else
      {
	      Application->MessageBox(UnicodeString("Incorrect PIN").w_str(),UnicodeString("Incorrect PIN").w_str(),MB_OK + MB_ICONINFORMATION);
      }
    }
    else
    {
       Application->MessageBox(UnicodeString("No Codes").w_str(),UnicodeString("Information").w_str(),MB_OK + MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

