//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "enum.h"
#include "GetMoney.h"
#include "Printing.h"
#include "Comms.h"
#include "Processing.h"
#include "MMMessageBox.h"
#include "GUIScale.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
#include <memory>
#include "Paytype.h"
#include "DrinkCommandData.h"

TForm * TfrmControlTransaction::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TfrmControlTransaction::TfrmControlTransaction(TComponent* Owner,TPaymentTransaction &inCurrentTransaction)
	: TZForm(Owner), CurrentTransaction(inCurrentTransaction)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmControlTransaction::CreateParams(Controls::TCreateParams &params)
{ 
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
//---------------------------------------------------------------------------
TfrmControlTransaction *TfrmControlTransaction::Create(TForm* Owner,TPaymentTransaction &inCurrentTransaction)
{
	WinOwner = Owner;
	return new TfrmControlTransaction(Owner,inCurrentTransaction);
}
//---------------------------------------------------------------------------
void __fastcall TfrmControlTransaction::FormShow(TObject *Sender)
{
	FormResize(Sender);

    const int StdRows = 12;
	int MinRows = 4; // gap,Total and change,Rounding.
    const int MinGap = 7;
	for ( int i = 0 ; i < CurrentTransaction.PaymentsCount(); i++ )
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if(Payment->GetPay() != 0 || Payment->GetCashOut() != 0)
		{
            MinRows++;
            if(Payment->GetAdjustment() != 0)
			 {
				MinRows++;
             }
         }
     }

   const int PayName = 0;
   const int Cashout = 1;
   const int Cashpay = 2;

   sgPay->ColWidths[PayName] = pnlCashOutTitle->Left - sgPay->Left;
   sgPay->ColWidths[Cashout] = pnlPaymentAmount->Left - pnlCashOutTitle->Left;
   sgPay->ColWidths[Cashpay] = sgPay->ClientWidth - (sgPay->ColWidths[PayName] + sgPay->ColWidths[Cashout]);

   sgPay->DefaultRowHeight	= sgPay->ClientHeight / StdRows;

   if(MinRows > StdRows)
	{
      sgPay->RowCount = MinRows;
   }
   else
   {
      sgPay->RowCount = StdRows;
   }
   for (int i = 0; i < sgPay->RowCount ; i++)
   {
		sgPay->Cells[PayName][i] = "";
		sgPay->Cells[Cashout][i] = "";
		sgPay->Cells[Cashpay][i] = "";
	}

	int MoneyRow = 0;
    //Get count of
    int DisplayCounter = MinGap + (int)TGlobalSettings::Instance().EnableDisplayTax + (int)TGlobalSettings::Instance().EnableDisplayServiceCharge;
	for ( int i = 0 ; i <  CurrentTransaction.PaymentsCount(); i++ )
	{
		TPayment *Payment = CurrentTransaction.PaymentGet(i);
		if(Payment->GetPay() != 0 || Payment->GetCashOut() != 0)
		{
			sgPay->Cells[PayName][MoneyRow] = Payment->Name;
			sgPay->Cells[Cashout][MoneyRow] = CurrToStrF(Payment->GetCashOutTotal(),ffNumber,2) + " ";
			sgPay->Cells[Cashpay][MoneyRow] = CurrToStrF(Payment->GetPayTendered(),ffNumber,2) + " ";
			MoneyRow++;
		}
	}
	MoneyRow++;

	sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Product Purchases";
	sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
	sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF(CurrentTransaction.Money.TotalProduct,ffNumber,2) + " ";

    DisplayCounter--;

	sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Other Charges";
	sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
	sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF(CurrentTransaction.Money.TotalAdjustment,ffNumber,2) + " ";

    DisplayCounter--;

	sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Rounding";
	sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
	sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF(CurrentTransaction.Money.TotalRounding,ffNumber,2) + " ";
     DisplayCounter--;

    if(TGlobalSettings::Instance().EnableDisplayTax)
       {
          sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Tax";
          sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
	      sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF( getTotalTaxFromTransaction(CurrentTransaction), ffNumber,2) + " ";
           DisplayCounter--;
       }

    if(TGlobalSettings::Instance().EnableDisplayServiceCharge)
       {
            sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Service Charge";
            sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
            sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF( getServiceChargeFromTransaction(CurrentTransaction), ffNumber,2) + " ";
             DisplayCounter--;
       }
	if(CurrentTransaction.CreditTransaction)
	{
               if(TGlobalSettings::Instance().ShowCurrencySymbol) // MM-2827
                {
                   sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Total (" + CurrencyString + ")";  //MM-2827
                }
                else
                {
                  sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Total";
                }
		sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
		sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF(CurrentTransaction.Money.GrandTotal,ffNumber,2) + " ";
	}
	else
	{
               if(TGlobalSettings::Instance().ShowCurrencySymbol) // MM-2827
                {
                   sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Total (" + CurrencyString + ")";  //MM-2827
                }
                else
                {
                  sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Total";
                }
		sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
		sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF(CurrentTransaction.Money.GrandTotal +CurrentTransaction.Money.Change,ffNumber,2) + " ";
	}
    DisplayCounter--;
	sgPay->Cells[PayName][sgPay->RowCount-DisplayCounter] = "Change";
	sgPay->Cells[Cashout][sgPay->RowCount-DisplayCounter] = "";
	sgPay->Cells[Cashpay][sgPay->RowCount-DisplayCounter] = CurrToStrF(CurrentTransaction.Money.Change,ffNumber,2) + " ";


        btnBack->Enabled = CurrentTransaction.TypeOfSale == RegularSale;
	btnPrintBill->Enabled = (!Receipt->AlwaysPrintReceiptTenderedSales) && (CurrentTransaction.TypeOfSale != NonChargableSale);
}


//---------------------------------------------------------------------------
void __fastcall TfrmControlTransaction::btnCloseBillClick(TObject *Sender)
{
   UserOption = eClose;
   Close();
//   Preliminary = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmControlTransaction::FormResize(TObject *Sender)
{
   //	if (Tag != Screen->Width)
  //{
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
 //  }
  // GUIScale::ChildrenTop(Panel1, 0.9);
  GUIScale::ParentHeight(pnlPayment, 0.9);

}
//---------------------------------------------------------------------------
void __fastcall TfrmControlTransaction::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmControlTransaction::stPrintBillClick(TObject *Sender)
{
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		MessageBox("Please select a receipt printer from Setup first.", "Print error", MB_OK + MB_ICONERROR);
   }
   else
   {
      UserOption = eCloseandPrint;
   }
   Close();
}
//---------------------------------------------------------------------------


eUserOptions TfrmControlTransaction::RetrieveUserOptions()
{
   Reset();
	btnCloseBill->Enabled = true;
	btnCloseBill->Font->Color = clWhite;

	btnPrintBill->Visible = true;
	btnPrintBill->Enabled = true;
	btnPrintBill->Font->Color = clWhite;

	ShowModal();
	return UserOption;
}

eUserOptions TfrmControlTransaction::CloseBill()
{
   Reset();
	btnCloseBill->Enabled = true;
   btnCloseBill->Font->Color = clWhite;

	btnPrintBill->Visible = false;
	btnPrintBill->Enabled = false;
   btnPrintBill->Font->Color = clWhite;

   ShowModal();
   return UserOption;
}

void TfrmControlTransaction::Reset()
{
	btnCloseBill->Enabled = false;
   btnCloseBill->Font->Color = clGray;

   btnPrintBill->Enabled = false;
   btnPrintBill->Font->Color = clGray;

   pnlPayment->Visible = true;
	UserOption = eNone;
}

void __fastcall TfrmControlTransaction::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
   if(UserOption == eNone)
   {
      CanClose = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmControlTransaction::btnBackClick(TObject *Sender)
{
   UserOption = eBack;
   Close();
}
//---------------------------------------------------------------------------

//Overwrite grid drawing to set alignment of cashpay column to left
void __fastcall TfrmControlTransaction::sgPayDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
  sgPay->Canvas->Font->Charset=DEFAULT_CHARSET;
  sgPay->Canvas->Font->Color = clWindowText;
  sgPay->Canvas->Font->Name="Tahoma";
  sgPay->Canvas->Brush->Color = RGB(218,218,218);
  UnicodeString CellContent = sgPay->Cells[ACol][ARow];
  if(ARow == sgPay->RowCount-1)
    {
        sgPay->Canvas->Font->Size=17;
        sgPay->Canvas->Font->Style= TFontStyles() << fsBold;
    }
  else
    {
        sgPay->Canvas->Font->Size=16;
        sgPay->Canvas->Font->Style= TFontStyles();
    }

  sgPay->Canvas->FillRect(Rect);
  if(ACol == 0)
    {
       sgPay->Canvas->TextRect(Rect, Rect.Left + 5 , Rect.Top, CellContent);
    }
   else
    {
         Word SavedAlign = SetTextAlign(sgPay->Canvas->Handle,TA_RIGHT);
       sgPay->Canvas->TextRect(Rect, Rect.Right - 5, Rect.Top, CellContent);
       SetTextAlign(sgPay->Canvas->Handle, SavedAlign);
    }
}
//---------------------------------------------------------------------------

Currency TfrmControlTransaction::getTotalTaxFromTransaction( TPaymentTransaction &inCurrentTransaction )
{
    Currency tax = CurrentTransaction.Money.ProductGSTContent;

    if( TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue )
    {
        // ProductGSTContent figure from TMoney does not have service charge tax in it.
        // if the setting is set to true, then we are to add the service charge tax portion
        tax += CurrentTransaction.Money.ServiceChargeTax;
    }

    return RoundToNearest(tax, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
}
//---------------------------------------------------------------------------

Currency TfrmControlTransaction::getServiceChargeFromTransaction( TPaymentTransaction &inCurrentTransaction )
{
    Currency serviceCharge = CurrentTransaction.Money.ServiceCharge;

    if( !TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue )
    {
        serviceCharge += CurrentTransaction.Money.ServiceChargeTax;
    }

    return RoundToNearest(serviceCharge, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
}
//---------------------------------------------------------------------------
