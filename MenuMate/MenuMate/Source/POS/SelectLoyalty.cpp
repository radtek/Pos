//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectLoyalty.h"
#include "Data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectLoyalty *frmSelectLoyalty;
//---------------------------------------------------------------------------
__fastcall TfrmSelectLoyalty::TfrmSelectLoyalty(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLoyalty::FormShow(TObject *Sender)
{
   FormResize(Sender);
	SearchString = "";
   edSearch->Text = "";
   frmData->IBLoyalCustomer->Open();

   if (frmData->IBLoyalCustomer->RecordCount == 0)
   {
   	btnLogin->Enabled = false;
   }
   else
   {
   	btnLogin->Enabled = true;
   }

   StringGrid1->RowCount = 1;
   StringGrid1->Cells[0][0] = "";
   CustomerLKs.clear();
   int Row = 0;
   for (frmData->IBLoyalCustomer->First(); !frmData->IBLoyalCustomer->Eof; frmData->IBLoyalCustomer->Next())
   {
      StringGrid1->RowCount++;
      CustomerLKs.push_back(frmData->IBLoyalCustomer->FieldByName("USER_KEY")->AsInteger);
      AnsiString Name = frmData->IBLoyalCustomer->FieldByName("NAME")->AsString.LowerCase();
      Name = Name.SubString(1, 1).UpperCase() + Name.SubString(2, Name.Length() - 1);
   	StringGrid1->Cells[0][Row++] = Name;
   }
   if(StringGrid1->RowCount > 1)
   {
      StringGrid1->RowCount--;
   }
   StringGrid1->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::btn7Click(TObject *Sender)
{
	if (SearchString == "")
   {
   	SearchString = ((TBitBtn *)Sender)->Caption;
   }
   else
   {
   	SearchString += ((TBitBtn *)Sender)->Caption.LowerCase();
   }
   StringGrid1->OnClick = NULL;
   try
   {
      edSearch->Text = SearchString;
      Variant locvalues = SearchString;
      if(frmData->IBLoyalCustomer->Locate("NAME", locvalues, TLocateOptions() << loPartialKey << loCaseInsensitive))
      {
         for (unsigned int i=0; i<CustomerLKs.size(); i++)
         {
            if (CustomerLKs[i] == frmData->IBLoyalCustomer->FieldByName("USER_KEY")->AsInteger)
            {
               StringGrid1->Row = i;
               break;
            }
         }
      }
   }
   __finally
   {
   	StringGrid1->OnClick = StringGrid1Click;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::BitBtn17Click(TObject *Sender)
{
   StringGrid1->OnClick = NULL;
   try
   {
      SearchString = SearchString.SubString(1, SearchString.Length() - 1);
      edSearch->Text = SearchString;
      Variant locvalues = SearchString;
      frmData->IBLoyalCustomer->Locate("NAME", locvalues, TLocateOptions() << loPartialKey);
      for (unsigned int i=0; i<CustomerLKs.size(); i++)
      {
         if (CustomerLKs[i] == frmData->IBLoyalCustomer->FieldByName("USER_KEY")->AsInteger)
         {
            StringGrid1->Row = i;
            break;
         }
      }
   }
   __finally
   {
   	StringGrid1->OnClick = StringGrid1Click;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
   	ScaleBy(Screen->Width, Temp);
   }
   StringGrid1->ColWidths[0] = StringGrid1->ClientWidth;
}

//---------------------------------------------------------------------------
void __fastcall TfrmSelectLoyalty::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::btnLoginClick(TObject *Sender)
{
	CustomerLK = CustomerLKs[StringGrid1->Row];
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::btnExitClick(TObject *Sender)
{
	CustomerLK = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::Button2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	SendMessage(StringGrid1->Handle, WM_VSCROLL, SB_LINEUP, 0L);
	TableScrollUp = true;
   Timer1->Interval = 60;
   Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::Button2MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::Button1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	SendMessage(StringGrid1->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
	TableScrollUp = false;
   Timer1->Interval = 60;
   Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::Button1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::Timer1Timer(TObject *Sender)
{
   Timer1->Interval = 60;
   if (TableScrollUp)
   {
		SendMessage(StringGrid1->Handle, WM_VSCROLL, SB_LINEUP, 0L);
   }
   else
   {
		SendMessage(StringGrid1->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectLoyalty::StringGrid1Click(TObject *Sender)
{
	SearchString = "";
   edSearch->Text = "";
}
//---------------------------------------------------------------------------


void __fastcall TfrmSelectLoyalty::FormPaint(TObject *Sender)
{
 	TDeviceRealTerminal::Instance().DemoModePaint(this);	
}
//---------------------------------------------------------------------------

