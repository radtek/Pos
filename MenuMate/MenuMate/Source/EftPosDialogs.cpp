//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EftPosDialogs.h"
#include "GUIScale.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmEftPos::TfrmEftPos(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmEftPos::tbSigYesClick(TObject *Sender)
{
	ModalResult = mrYes;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEftPos::tbSigNoClick(TObject *Sender)
{
	ModalResult = mrNo;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmEftPos::FormResize(TObject *Sender)
{
  //	if (Tag != Screen->Width)
 //  {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
 //  }

   if(this->Height > Screen->Height)
   {
   	this->Height = Screen->Height;
        pnlSuccess->Top = 100;
        GUIScale::ParentHeight(pnlSuccess, 0.8);
        GUIScale::ChildrenTop(pnlSuccess, 0.85);
        lbeElectronicTransType->Top = lbeElectronicTransType->Top * 0.9;
        lbeSuccess->Top = lbeSuccess->Top * 0.9;
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmEftPos::FormShow(TObject *Sender)
{
	FormResize(Sender);
}
//---------------------------------------------------------------------------

TModalResult TfrmEftPos::SignatureOk()
{
	pnlSuccess->Visible = false;
	pnlSignature->Visible = true;
	TModalResult Result = ShowModal();
	return Result;
}


TModalResult TfrmEftPos::TransactionOk(TPayment *Payment)
{
   lbeElectronicTransType->Caption = Payment->Name;
   stEftType->Caption = Payment->Name;
	stEFTMoney->Caption = CurrToStrF(Payment->GetPayTendered(), ffCurrency, CurrencyDecimals) + " ";
	stEftCashOut->Caption = CurrToStrF(Payment->GetCashOutTotal(), ffCurrency, CurrencyDecimals) + " ";
	stEftTotal->Caption = CurrToStrF(Payment->GetPayTendered() + Payment->GetCashOutTotal(), ffCurrency, CurrencyDecimals) + " ";
	pnlSuccess->Visible = true;
	pnlSignature->Visible = false;
	TModalResult Result = ShowModal();
	return Result;
}

TModalResult TfrmEftPos::TransactionOk(TPayment *Payment,AnsiString Caption)
{
	lbeElectronicTransType->Caption = Caption;
   stEftType->Caption = Payment->Name;
	stEFTMoney->Caption = CurrToStrF(Payment->GetPayTendered(), ffCurrency, CurrencyDecimals) + " ";
	stEftCashOut->Caption = CurrToStrF(Payment->GetCashOutTotal(), ffCurrency, CurrencyDecimals) + " ";
	stEftTotal->Caption = CurrToStrF(Payment->GetPayTendered() + Payment->GetCashOutTotal(), ffCurrency, CurrencyDecimals) + " ";
	pnlSuccess->Visible = true;
	pnlSignature->Visible = false;
	TModalResult Result = ShowModal();
	return Result;
}

