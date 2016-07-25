//---------------------------------------------------------------------------


#pragma hdrstop

#include "XeroInvoiceBuilder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TXeroInvoiceBuilder::TXeroInvoiceBuilder()
{

}
//---------------------------------------------------------------------------
TXeroInvoiceBuilder::~TXeroInvoiceBuilder()
{
	ClearXeroInvoice();
}
//---------------------------------------------------------------------------

bool TXeroInvoiceBuilder::BuildXeroInvoice( AnsiString                           inContactName,
                                            AnsiString                           inInvoiceNumber,
                                            AnsiString                           inInvoiceRef,
                                            TDateTime                            inInvoiceDate,
                                            TDateTime                            inInvoiceDueDate,
                                            std::vector<Stock::TInvoiceItemInfo> inInvoiceItems )
{
	 ClearXeroInvoice();

	 return CreateXeroInvoice( inContactName, inInvoiceNumber, inInvoiceRef, inInvoiceDate, inInvoiceDueDate, inInvoiceItems );
}
//---------------------------------------------------------------------------

//............................
//  Protected Methods
//............................
void TXeroInvoiceBuilder::ClearXeroInvoice()
{
	 if( FXeroInvoice )
	 {
		  delete FXeroInvoice;
	 }
}
//---------------------------------------------------------------------------
bool TXeroInvoiceBuilder::CreateXeroInvoice( AnsiString                           inContactName,
                                             AnsiString                           inInvoiceNumber,
                                             AnsiString                           inInvoiceRef,
                                             TDateTime                            inInvoiceDate,
                                             TDateTime                            inInvoiceDueDate,
                                             std::vector<Stock::TInvoiceItemInfo> inInvoiceItems )
{
	FXeroInvoice = new TXeroInvoice();

	FXeroInvoice->InvoiceType                 = xitACCPAY;
	FXeroInvoice->InvoiceNumber               = inInvoiceNumber;
	FXeroInvoice->InvoiceReference            = inInvoiceRef;
	FXeroInvoice->InvoiceDate                 = inInvoiceDate;
	FXeroInvoice->InvoiceDueDate              = inInvoiceDueDate;
	FXeroInvoice->InvoiceContact->ContactName = inContactName;

	//...........................................

    for( int i = 0; i < inInvoiceItems.size(); i++ )
	{
		AddItemToXeroInvoice( FXeroInvoice, inInvoiceItems[i] );
	}

	//:::::::::::::::::::::::::::::::::::::::::::::

	return FXeroInvoice->InvoiceItemCount > 0;
}
//---------------------------------------------------------------------------
void TXeroInvoiceBuilder::AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, Stock::TInvoiceItemInfo inItem )
{
	AnsiString itemCodeStr    = inItem.GLCode;
	AnsiString unitAmountStr  = FormatFloat( "0.00", inItem.Supplier_Unit_Cost );
	AnsiString qtyStr         = FormatFloat( "0.00", inItem.Order_Qty );
	AnsiString taxStr         = FormatFloat( "0.00", inItem.Total_GST );

	FXeroInvoice->AddItem( itemCodeStr, inItem.Description, unitAmountStr.ToDouble(), taxStr.ToDouble(), qtyStr.ToDouble() );
}
//---------------------------------------------------------------------------

