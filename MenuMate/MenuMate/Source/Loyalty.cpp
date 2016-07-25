//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loyalty.h"
#include "Data.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLoyalty *frmLoyalty;
//---------------------------------------------------------------------------
__fastcall TfrmLoyalty::TfrmLoyalty(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyalty::FormShow(TObject *Sender)
{
   frmData->IBLoyalCustomer->Open();
	FormResize(this);
   DBGrid1->SetFocus();
	wrkPayAmount = 0;
	edDollars->Text = FloatToStrF(wrkPayAmount, ffCurrency, 18, CurrencyDecimals);
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyalty::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyalty::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyalty::imgExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoyalty::btn7Click(TObject *Sender)
{
	TBitBtn *Button = (TBitBtn *)Sender;

   if (long(double(wrkPayAmount)) / 100000 != 0)
   {
      return;
   }
   if (Button->Caption == "00")
   {
      wrkPayAmount = double(wrkPayAmount) * 100;
      edDollars->Text = FloatToStrF(wrkPayAmount, ffCurrency, 18, CurrencyDecimals);
   }
   else
   {
      wrkPayAmount = double(wrkPayAmount) * 10;
      wrkPayAmount += StrToCurr(Button->Caption) / 100;
      edDollars->Text = FloatToStrF(wrkPayAmount, ffCurrency, 18, CurrencyDecimals);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoyalty::btnClearClick(TObject *Sender)
{
	wrkPayAmount = 0;
	edDollars->Text = FloatToStrF(wrkPayAmount, ffCurrency, 18, CurrencyDecimals);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoyalty::DBEdit1Enter(TObject *Sender)
{
	DBGrid1->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoyalty::FormCreate(TObject *Sender)
{
   imgExit->Picture->Bitmap = frmMain->imgExit->Picture->Bitmap;
}
//---------------------------------------------------------------------------



void __fastcall TfrmLoyalty::FormPaint(TObject *Sender)
{
 	TDeviceRealTerminal::Instance().DemoModePaint(this);	
}
//---------------------------------------------------------------------------

