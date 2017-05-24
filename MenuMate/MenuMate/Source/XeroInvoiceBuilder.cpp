//---------------------------------------------------------------------------

#pragma hdrstop

#include "XeroInvoiceBuilder.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "enumTab.h"
#include "DBTables.h"
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
bool TXeroInvoiceBuilder::BuildXeroInvoice( TPaymentTransaction& inPaymentTransaction )
{
	 ClearXeroInvoice();

	 if( ChargeToXero( inPaymentTransaction ) )
	 {
		 return CreateXeroInvoice( inPaymentTransaction );
	 }

	 return false;
}
//---------------------------------------------------------------------------
bool TXeroInvoiceBuilder::BuildXeroInvoice(TXeroInvoiceDetail&  XeroInvoiceDetail)
{
   ClearXeroInvoice();
   return CreateXeroInvoice(XeroInvoiceDetail);
}
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
bool TXeroInvoiceBuilder::ChargeToXero( TPaymentTransaction& inPaymentTransaction )
{
	bool result = false;

	//..............................................................

	for (int i = 0; i < inPaymentTransaction.PaymentsCount(); i++)
	{
		FXeroPayment =  inPaymentTransaction.PaymentGet(i);

		if((FXeroPayment->GetPaymentAttribute(ePayTypeChargeToXero))  && FXeroPayment->GetPay() != 0)
		{
			 result = true;
			 break;
		}
	}

	//..............................................................

	return result;
}
//---------------------------------------------------------------------------
bool TXeroInvoiceBuilder::CreateXeroInvoice( TPaymentTransaction& inPaymentTransaction )
{
	FPercentageToXero = GetTotalAmountToXero( FXeroPayment ) / GetTotalAmount( inPaymentTransaction );
    int tableno;
    AnsiString customerName;
    AnsiString customerPhone;
    bool isCustomerNamePresent;
    AnsiString partyName ;
    int contactKey;

    bool orderSaveOnTable = false;
	//:::::::::::::::::::::::::::::::::::::::::::

	FXeroInvoice = new TXeroInvoice();
    FXeroInvoice->InvoiceType = inPaymentTransaction.CreditTransaction ? xitACCRECCREDIT : xitACCREC;
    if(TGlobalSettings::Instance().AwaitingPaymentInXero)
    {
        FXeroInvoice->InvoiceStatus               = "AUTHORISED";
    }
	FXeroInvoice->InvoiceNumber               = inPaymentTransaction.InvoiceNumber;
	FXeroInvoice->InvoiceReference            = "";
	FXeroInvoice->InvoiceDate                 = Now();
	FXeroInvoice->InvoiceDueDate              = Now();
	FXeroInvoice->InvoiceContact->ContactName = FXeroPayment->InvoiceContact.Name + " " + FXeroPayment->InvoiceContact.Surname;


    if(inPaymentTransaction.ChitNumber.ChitNumber != NULL && inPaymentTransaction.ChitNumber.ChitNumber != "")
     {
       AnsiString chitDetail =  inPaymentTransaction.ChitNumber.Name + " " + inPaymentTransaction.ChitNumber.ChitNumber;
       FXeroInvoice->AddItem( "","Chit No:- "+ chitDetail, 0, 0, 1);
       if(inPaymentTransaction.ChitNumber.IsCaptureCustomerNameAndPhone)
       {
         if(TGlobalSettings::Instance().TabPrintName != "")
           FXeroInvoice->AddItem( "","Customer Name:- " + TGlobalSettings::Instance().TabPrintName , 0, 0, 1);
         if(TGlobalSettings::Instance().TabPrintPhone != "")
           FXeroInvoice->AddItem( "","Customer Phone:- " + TGlobalSettings::Instance().TabPrintPhone, 0, 0, 1);
       }
     }

    if(inPaymentTransaction.Orders->Count > 0)
     {
         TItemComplete *Order = ( TItemComplete* )inPaymentTransaction.Orders->Items[0];
         if(Order->TabType == TabTableSeat)
          {
              partyName = TDBTables::GetPartyName(inPaymentTransaction.DBTransaction,Order->TableNo);

              if(partyName != NULL && partyName != "")
               {
                 FXeroInvoice->AddItem( "", "Party Name:- " + partyName , 0, 0, 1);
               }
              else
               {
                 FXeroInvoice->AddItem( "", "Table No:- " + IntToStr(Order->TableNo) , 0, 0, 1);
               }
          }
          else if(inPaymentTransaction.PartyName != NULL && inPaymentTransaction.PartyName != "")
          {
             FXeroInvoice->AddItem( "", "Party Name:- " + inPaymentTransaction.PartyName , 0, 0, 1);
          }
    }
    // same Items with similar properties should show in one node of XML
    // with quantity as total
    CompressItemForInvoice(FXeroInvoice,inPaymentTransaction);

	return FXeroInvoice->InvoiceItemCount > 0;
}
//---------------------------------------------------------------------------
void TXeroInvoiceBuilder::AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, TItemMinorComplete* inOrder )
{
    Currency TotalTax = inOrder->TotalTax_BillCalc() + inOrder->ServiceCharge_BillCalc();
	AnsiString itemCodeStr    = inOrder->Categories->FinancialCategoryGLCode;
	AnsiString totalAmountStr = FormatFloat( "0.0000", inOrder->BillCalcResult.FinalPrice);
	AnsiString qtyStr         = FormatFloat( "0.0000", inOrder->GetQty() * FPercentageToXero );
	AnsiString taxStr         = FormatFloat( "0.0000", TotalTax );
	//................................................

	AnsiString originalQtyStr = FormatFloat( "0.0000", inOrder->GetQty() );
	AnsiString unitAmountStr  = CalcAsString( totalAmountStr, originalQtyStr, taxStr );
	//................................................
    FXeroInvoice->AddItem( itemCodeStr, inOrder->Item, unitAmountStr.ToDouble(), taxStr.ToDouble(), qtyStr.ToDouble() );

}
//---------------------------------------------------------------------------
AnsiString TXeroInvoiceBuilder::CalcAsString(AnsiString inPrice, AnsiString inQty, AnsiString &inTax )
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
double TXeroInvoiceBuilder::GetTotalAmountToXero( TPayment* inXeroPayment )
{
	return inXeroPayment->GetPayTendered();
}
//---------------------------------------------------------------------------
double TXeroInvoiceBuilder::GetTotalAmount( TPaymentTransaction& inPaymentTransaction )
{
	//return inPaymentTransaction.Money.Total + inPaymentTransaction.Money.Surcharge + inPaymentTransaction.Money.Discount;
    return inPaymentTransaction.Money.GrandTotal;
}

bool TXeroInvoiceBuilder::CreateXeroInvoice(TXeroInvoiceDetail& XeroInvoiceDetail)
{
   FXeroInvoice = new TXeroInvoice();
   FXeroInvoice->InvoiceType                 = xitACCREC;
   FXeroInvoice->InvoiceStatus               = "AUTHORISED";
   FXeroInvoice->InvoiceNumber               = XeroInvoiceDetail.InvoiceNumber;
   FXeroInvoice->InvoiceReference            = XeroInvoiceDetail.InvoiceReference;
   FXeroInvoice->InvoiceDate                 = XeroInvoiceDetail.InvoiceDate;
   FXeroInvoice->InvoiceDueDate              = XeroInvoiceDetail.InvoiceDate;
   FXeroInvoice->InvoiceContact->ContactName = XeroInvoiceDetail.ContactName;
   double invoiceTotal = 0;
   CheckRoundingAmount(XeroInvoiceDetail);

    for (std::vector<TXeroPayTypeDetail>::iterator it = XeroInvoiceDetail.XeroPayTypeDetails.begin() ;
          it != XeroInvoiceDetail.XeroPayTypeDetails.end(); ++it)
     {
        invoiceTotal += it->UnitAmount;
        AddPaymentToXeroInvoice(FXeroInvoice,*it);
     }

     for (std::vector<TXeroCategoryDetail>::iterator it = XeroInvoiceDetail.XeroCategoryDetails.begin() ;
          it != XeroInvoiceDetail.XeroCategoryDetails.end(); ++it)
     {
           AddItemToXeroInvoice(FXeroInvoice,*it);
     }
    FXeroInvoice->InvoiceType = invoiceTotal < 0 ? xitACCRECCREDIT : xitACCREC;;

	return FXeroInvoice->InvoiceItemCount > 0;
}

void TXeroInvoiceBuilder::AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, TXeroCategoryDetail& PayTypeDetail )
{
    AnsiString itemCodeStr    = PayTypeDetail.AccountCode;
	AnsiString totalAmountStr = FormatFloat( "0.0000", PayTypeDetail.UnitAmount );
	AnsiString qtyStr         = FormatFloat( "0.0000", PayTypeDetail.Quantity);
	AnsiString taxStr         = FormatFloat( "0.0000", PayTypeDetail.TaxAmount );
	AnsiString originalQtyStr = FormatFloat( "0.0000", PayTypeDetail.Quantity );
	AnsiString unitAmountStr  = FormatFloat( "0.0000", PayTypeDetail.UnitAmount );;

	FXeroInvoice->AddItem( itemCodeStr, PayTypeDetail.Description, unitAmountStr.ToDouble(), taxStr.ToDouble(), qtyStr.ToDouble() );
}

void TXeroInvoiceBuilder::AddPaymentToXeroInvoice( TXeroInvoice* inXeroInvoice, TXeroPayTypeDetail& PayTypeDetail )
{
       AnsiString itemCodeStr    = PayTypeDetail.AccountCode;
        AnsiString totalAmountStr = FormatFloat( "0.0000", PayTypeDetail.UnitAmount );
        AnsiString taxStr         = FormatFloat( "0.0000", PayTypeDetail.TaxAmount );
        AnsiString unitAmountStr  = FormatFloat( "0.0000", PayTypeDetail.UnitAmount );;

        FXeroInvoice->AddPayment( itemCodeStr, PayTypeDetail.Description, unitAmountStr.ToDouble(), taxStr.ToDouble() );

}
// All items and sides are to be stored in vector
void TXeroInvoiceBuilder::CompressItemForInvoice(TXeroInvoice *FXeroInvoice,TPaymentTransaction& inPaymentTransaction)
{
    std::vector<NodeItem> nodeItems;
    for( int i = 0; i < inPaymentTransaction.Orders->Count; i++ )
	{
           NodeItem xeroNodeItem;
           TItemComplete *item = (TItemComplete *)inPaymentTransaction.Orders->Items[i];

           Currency totalTax = item->TotalTax_BillCalc()+ item->ServiceCharge_BillCalc();
           AnsiString totalAmountStr = FormatFloat( "0.0000", item->BillCalcResult.FinalPrice);
           AnsiString qtyStr         = FormatFloat( "0.0000", item->GetQty() * FPercentageToXero );
           AnsiString taxStr         = FormatFloat( "0.0000", totalTax );
           AnsiString originalQtyStr = FormatFloat( "0.0000", item->GetQty() );
           AnsiString unitAmountStr  = CalcAsString( totalAmountStr, originalQtyStr, taxStr );

           xeroNodeItem.ItemCode = item->Categories->FinancialCategoryGLCode;
           xeroNodeItem.ItemName = item->Item;
           xeroNodeItem.UnitAmount = unitAmountStr;
           xeroNodeItem.TaxAmount = taxStr;
           xeroNodeItem.Qty = qtyStr;

           ///Add Node to vector if not exist there else update quantity and tax
           AddItemToCollection(xeroNodeItem, nodeItems);

           for( int j = 0; j < item->SubOrders->Count; j++ )
           {
                TItemCompleteSub *SubOrderImage = item->SubOrders->SubOrderGet(j);
                NodeItem xeroSubNodeItem;

                Currency totalTax = SubOrderImage->TotalTax_BillCalc() + SubOrderImage->ServiceCharge_BillCalc();
                AnsiString totalAmountStr = FormatFloat( "0.0000", SubOrderImage->BillCalcResult.FinalPrice);
                AnsiString qtyStr         = FormatFloat( "0.0000", SubOrderImage->GetQty() * FPercentageToXero );
                AnsiString taxStr         = FormatFloat( "0.0000", totalTax );
                AnsiString originalQtyStr = FormatFloat( "0.0000", SubOrderImage->GetQty() );
                AnsiString unitAmountStr  = CalcAsString( totalAmountStr, originalQtyStr, taxStr );

               xeroSubNodeItem.ItemCode = SubOrderImage->Categories->FinancialCategoryGLCode;
               xeroSubNodeItem.ItemName = SubOrderImage->Item;
               xeroSubNodeItem.UnitAmount = unitAmountStr;
               xeroSubNodeItem.TaxAmount = taxStr;
               xeroSubNodeItem.Qty = qtyStr;

               ///Add Node to vector if not exist there else update quantity and tax
               AddItemToCollection(xeroSubNodeItem, nodeItems);
           }
    }

    //Add Item To XML
   AddItemToInvoiceXML(FXeroInvoice , nodeItems) ;
}

void TXeroInvoiceBuilder::CheckRoundingAmount(TXeroInvoiceDetail& XeroInvoiceDetail)
{
     for (std::vector<TXeroCategoryDetail>::iterator it = XeroInvoiceDetail.XeroCategoryDetails.begin() ;
          it != XeroInvoiceDetail.XeroCategoryDetails.end(); ++it)
     {
          AnsiString description = it->Description;
          if(description == "ROUNDING")
          {
             double unitamount = it->UnitAmount;
             double roundAmount = 0;
             unitamount = RoundTo(unitamount, -4);

             if(RoundTo(fabs((it->UnitAmount)), -2) > 0.01)
             {
                break;
             }

             if(unitamount < 0)
             {
                if((unitamount <= -0.0000 && unitamount > -0.0200))
                {
                    roundAmount = it->UnitAmount;

                    AdjustRoundingToCategoriesTotal(XeroInvoiceDetail, roundAmount);

                    XeroInvoiceDetail.XeroCategoryDetails.erase(it);
                    break;
                }

             }
             else
             {
                if(unitamount >= 0.0000 && unitamount < 0.0200)
                {
                    roundAmount = it->UnitAmount;

                    AdjustRoundingToCategoriesTotal(XeroInvoiceDetail, roundAmount);

                    XeroInvoiceDetail.XeroCategoryDetails.erase(it);
                    break;
                }

             }
          }
     }
}

//-------------------------------------------------------------------------------------------------
void TXeroInvoiceBuilder::AddItemToCollection(NodeItem xeroNodeItem, std::vector<NodeItem> &nodeItems)
{
    double qty = 0;
    double totalTax = 0;
    bool isFound = false;

    //Iterate vector and compare the received item whether it exist in the vector or not. if exist then update it's quantity and tax
    //otherwise add it to vector.
    for(int index = 0; index < nodeItems.size() ; ++index)
    {
         if((nodeItems[index].ItemCode == xeroNodeItem.ItemCode) &&(nodeItems[index].ItemName == xeroNodeItem.ItemName) &&
                        (nodeItems[index].UnitAmount == xeroNodeItem.UnitAmount))
         {
            qty =  StrToFloat(nodeItems[index].Qty.c_str()) + StrToFloat(xeroNodeItem.Qty.c_str());
            totalTax = StrToFloat(nodeItems[index].TaxAmount.c_str()) + StrToFloat(xeroNodeItem.TaxAmount.c_str());
            nodeItems[index].Qty = qty;
            nodeItems[index].TaxAmount = totalTax;
            isFound = true;
            break;
         }
    }
    if(!isFound)
    {
        nodeItems.push_back(xeroNodeItem);
    }
}
//----------------------------------------------------------------------------------------------------------
void TXeroInvoiceBuilder::AddItemToInvoiceXML(TXeroInvoice *FXeroInvoice, std::vector<NodeItem> nodeItems)
{
    for(std::vector<NodeItem>::iterator it = nodeItems.begin() ; it != nodeItems.end() ; ++it)
    {
        // Items are added to Invoice XML
        FXeroInvoice->AddItem( it->ItemCode, it->ItemName, it->UnitAmount.ToDouble(), it->TaxAmount.ToDouble(), it->Qty.ToDouble() );
    }
}

//----------------------------------------------------------------------------------------------------------
void TXeroInvoiceBuilder::AdjustRoundingToCategoriesTotal(TXeroInvoiceDetail& XeroInvoiceDetail, double roundingAmount)
{
     for (std::vector<TXeroCategoryDetail>::iterator it = XeroInvoiceDetail.XeroCategoryDetails.begin() ;
          it != XeroInvoiceDetail.XeroCategoryDetails.end(); ++it)
     {
        it->UnitAmount += roundingAmount;
        break;
     }
}





