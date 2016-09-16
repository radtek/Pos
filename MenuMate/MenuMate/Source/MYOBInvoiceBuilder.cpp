//---------------------------------------------------------------------------

#pragma hdrstop

#include "MYOBInvoiceBuilder.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "enumTab.h"
#include "DBTables.h"
#include "MMMessageBox.h""
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TMYOBInvoiceBuilder::TMYOBInvoiceBuilder()
{

}
//---------------------------------------------------------------------------
TMYOBInvoiceBuilder::~TMYOBInvoiceBuilder()
{
	ClearMYOBInvoice();
}

//---------------------------------------------------------------------------
bool TMYOBInvoiceBuilder::BuildMYOBInvoice(TMYOBInvoiceDetail&  MYOBInvoiceDetail)
{
   ClearMYOBInvoice();
   return CreateMYOBInvoice(MYOBInvoiceDetail);
}
//............................
//  Protected Methods
//............................
void TMYOBInvoiceBuilder::ClearMYOBInvoice()
{
	 if( FMYOBInvoice )
	 {
		  delete FMYOBInvoice;
	 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
AnsiString TMYOBInvoiceBuilder::CalcAsString(AnsiString inPrice, AnsiString inQty, AnsiString &inTax )
{
	// This is the same algorythm as the one we use in Summa Pro C# app to calculate unit price
	// result = (B/A) - (D/A) = Unit Price
	// A      = Quantity: inQty
	// B      = Net Price: inPrice
	// D      = Tx: inTax

	AnsiString result = "0.0000";

	//....................................................
    double qty = inQty.ToDouble();
	double price = inPrice.ToDouble();
	double tax = inTax.ToDouble();
	double resultDouble = (price - tax) / qty;
    tax = price -  (resultDouble * qty);
    inTax =  FormatFloat( "0.0000", tax );
 	if (qty != 0)
	{
		if (qty > 0)
		{
			// NORMAL SALE AND SALE OF AN ITEM WITH A NEGATIVE PRICE
			result = FormatFloat( "0.0000", resultDouble );
		}
		else // REFUND!!!!!!
		{
			result = FormatFloat( "0.0000", std::abs( resultDouble ) );
		}
	}
	else // QUANTITY = 0
	{
		result = FormatFloat( "0.0000", price );
	}

	//....................................................

	return result;
}
//---------------------------------------------------------------------------

bool TMYOBInvoiceBuilder::CreateMYOBInvoice(TMYOBInvoiceDetail& MYOBInvoiceDetail)
{

   FMYOBInvoice = new TMYOBInvoice();
   FMYOBInvoice->InvoiceStatus               = "AUTHORISED";
   FMYOBInvoice->InvoiceNumber               = MYOBInvoiceDetail.InvoiceNumber;
   FMYOBInvoice->InvoiceReference            = MYOBInvoiceDetail.InvoiceReference;
   FMYOBInvoice->InvoiceDate                 = MYOBInvoiceDetail.InvoiceDate;
   FMYOBInvoice->InvoiceDueDate              = MYOBInvoiceDetail.InvoiceDate;
   FMYOBInvoice->InvoiceContact->ContactName = MYOBInvoiceDetail.CustomerName;
   FMYOBInvoice->JobCode                     = MYOBInvoiceDetail.JobCode;


     for (std::vector<TMYOBCategoryDetail>::iterator it = MYOBInvoiceDetail.MYOBCategoryDetails.begin() ;
          it != MYOBInvoiceDetail.MYOBCategoryDetails.end(); ++it)
     {
        AddItemToMYOBInvoice(FMYOBInvoice,*it);
     }

	return FMYOBInvoice->InvoiceItemCount > 0;
}

void TMYOBInvoiceBuilder::AddItemToMYOBInvoice( TMYOBInvoice* inMYOBInvoice, TMYOBCategoryDetail& PayTypeDetail )
{
    AnsiString glCode         = PayTypeDetail.GLCode;
    AnsiString description    = PayTypeDetail.Description;
	AnsiString totalAmountStr = FormatFloat( "0.00", PayTypeDetail.Amount );
	AnsiString taxStatus      = PayTypeDetail.TaxStatus;
    double taxAmount            = PayTypeDetail.TaxAmount;
	FMYOBInvoice->AddItem( glCode, description, totalAmountStr.ToDouble(), taxAmount,taxStatus);
}

