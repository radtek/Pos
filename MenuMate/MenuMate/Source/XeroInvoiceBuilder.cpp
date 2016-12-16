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

		if((FXeroPayment->Properties & ePayTypeChargeToXero)  && FXeroPayment->GetPay() != 0)
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
    MakeVectorForItems(FXeroInvoice,inPaymentTransaction);

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
    //if(CheckRoundingAmount(inOrder->Item, unitAmountStr.ToDouble()));
    //{
    FXeroInvoice->AddItem( itemCodeStr, inOrder->Item, unitAmountStr.ToDouble(), taxStr.ToDouble(), qtyStr.ToDouble() );
    //}
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

   roundAmount = 0;
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
        invoiceTotal += (it->UnitAmount + it->TaxAmount);
        invoiceTotal = RoundTo(invoiceTotal, -4);
        if(invoiceTotal == 0.01 || invoiceTotal == -0.01)
        {
           invoiceTotal += roundAmount;
           AddItemToXeroInvoice(FXeroInvoice,*it, true);
        }
        else
        {
           AddItemToXeroInvoice(FXeroInvoice,*it);
        }
     }
    FXeroInvoice->InvoiceType = invoiceTotal < 0 ? xitACCRECCREDIT : xitACCREC;;

	return FXeroInvoice->InvoiceItemCount > 0;
}

void TXeroInvoiceBuilder::AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, TXeroCategoryDetail& PayTypeDetail , bool isAddRounding)
{
    if(isAddRounding)
    {
       PayTypeDetail.UnitAmount += roundAmount;
    }
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
void TXeroInvoiceBuilder::MakeVectorForItems(TXeroInvoice *FXeroInvoice,TPaymentTransaction& inPaymentTransaction)
{
    NodeItems.erase(NodeItems.begin(),NodeItems.end());
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
           NodeItems.push_back(xeroNodeItem);
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
               NodeItems.push_back(xeroSubNodeItem);
            }
    }
    //The elements are to be modified in terms of quantity and Tax if same item
    // exists more than once with similar quantity
   ModifyVector(FXeroInvoice , NodeItems) ;
}
void TXeroInvoiceBuilder::ModifyVector(TXeroInvoice *FXeroInvoice,std::vector<NodeItem> nodeItems)
{
      bool erase = false;
      //storeIndex = "";
       double qty;
       double totalTax;
       AnsiString storeIndex = "";

      for(int i = 0; i < nodeItems.size()  ; i++)
      {

          totalTax=0;
          int j = 0;
           qty = 0;
             if(storeIndex.Pos(i) == 0)
             {
          for(std::vector<NodeItem>::iterator it = nodeItems.begin() ; it != nodeItems.end() ; ++it)
          {
             if((nodeItems.at(i).ItemCode == it->ItemCode) &&(nodeItems.at(i).ItemName == it->ItemName) &&(nodeItems.at(i).UnitAmount == it->UnitAmount))

              {
                  AnsiString tx= it->TaxAmount;
                  qty +=  StrToFloat(it->Qty.c_str());
                  nodeItems.at(i).Qty = qty;
                  try
                  {
                      totalTax += StrToFloat(tx.c_str());
                      nodeItems.at(i).TaxAmount = totalTax;
                  }
                  catch(Exception &e)
                  {

                  }

                  if(i != j)
                  {
                     // the indices which should be skipped while creating XML are stored
                     // because the quantity of first occurrence has been modified
                     storeIndex += ",";
                     storeIndex += j ;
                  }
              }
              j++;
          }
          }
      }
           storeIndex += ",";
           int index = 0;
          for(std::vector<NodeItem>::iterator it = nodeItems.begin() ; it != nodeItems.end() ; ++it)
          {
             AnsiString checkIndex = "";
             checkIndex = ",";
             checkIndex += index;
             checkIndex += ",";
             if(storeIndex.Pos(checkIndex) == 0)
             {
                // Items are added to Invoice XML
                FXeroInvoice->AddItem( it->ItemCode, it->ItemName, it->UnitAmount.ToDouble(), it->TaxAmount.ToDouble(), it->Qty.ToDouble() );
             }
             index ++;
          }
}

void TXeroInvoiceBuilder::CheckRoundingAmount(TXeroInvoiceDetail& XeroInvoiceDetail)
{
    //bool retVal = true;
     for (std::vector<TXeroCategoryDetail>::iterator it = XeroInvoiceDetail.XeroCategoryDetails.begin() ;
          it != XeroInvoiceDetail.XeroCategoryDetails.end(); ++it)
     {
          AnsiString description = it->Description;
          if(description == "ROUNDING")
          {
             double unitamount = it->UnitAmount;
             unitamount = RoundTo(unitamount, -4);
             if(unitamount == 0.01 || unitamount == -0.01)
             {
                roundAmount = unitamount;
                XeroInvoiceDetail.XeroCategoryDetails.erase(it);
                //XeroInvoiceDetail.XeroCategoryDetails.pop_back();
                break;
             }
          }
     }

    /*if(description == "ROUNDING")
    {
       if(roundingamount <= 0.01 && roundingamount >= -0.01)
       {
           retVal = false;
       }
    }*/
    //return retVal;
}





