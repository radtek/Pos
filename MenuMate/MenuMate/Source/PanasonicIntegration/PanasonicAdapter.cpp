//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "PanasonicAdapter.h"
#include "DBPanasonic.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

void TPanasonicAdapter::ConvertTransactionInfoToPanasonicInfo(TPaymentTransaction &paymentTransaction, long arcBillKey, UnicodeString receipt)
{
    try
    {

          //Convert TransactionInfo to panasonic Transaction info so that it can be directly posted to their db.
          ConverTransactionInfoToPanasonicTransactionInfo(paymentTransaction, arcBillKey, receipt);

           //Convert TransactionInfo to panasonic Item info so that it can be directly posted to their TItemList Table.
          ConvertTransactionInfoToPanasonicItemList(paymentTransaction, arcBillKey);

           //Convert TransactionInfo to panasonic product info .
          ConvertTransactionInfoToPanasonicProduct(paymentTransaction);

          //fill transaction db server info.
          ConverTransactionInfoToTransactionDBServerInfo(paymentTransaction);
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------------------------
long TPanasonicAdapter::GetSiteId(TPaymentTransaction &paymentTransaction)
{
    long Retval;
    TIBSQL *IBInternalQuery = paymentTransaction.DBTransaction.Query(paymentTransaction.DBTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT INTEGER_VAL FROM VARSPROFILE where VARSPROFILE.VARIABLES_KEY = :VARIABLES_KEY";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 9;
        IBInternalQuery->ExecQuery();
        Retval = IBInternalQuery->RecordCount > 0 ? IBInternalQuery->Fields[0]->AsInteger : 0;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return Retval;
}
//-------------------------------------------------------------------------------------------------------------------------
void TPanasonicAdapter::ConverTransactionInfoToPanasonicTransactionInfo(TPaymentTransaction &paymentTransaction, long arcBillKey, UnicodeString receipt)
{

      TPanasonicModels panasonicModel;

      panasonicModel.StoreId               = GetSiteId(paymentTransaction);
      panasonicModel.Terminald             = TDeviceRealTerminal::Instance().ID.Name;
      panasonicModel.OperatorId            = TDeviceRealTerminal::Instance().User.ContactID;
      panasonicModel.OperatorName          = TDeviceRealTerminal::Instance().User.Name;
      panasonicModel.CustomerId            = paymentTransaction.Membership.Member.ContactKey;
      panasonicModel.CustomerName          = paymentTransaction.Membership.Member.Name;
      panasonicModel.TransactionId         = paymentTransaction.InvoiceNumber;
      panasonicModel.TransactionType       = SecurityTypes[secBilledBy];
      panasonicModel.ProductListId         = arcBillKey;
      panasonicModel.TransactionAmount     = paymentTransaction.Money.RoundedGrandTotal;
      panasonicModel.AgeRestricted         = false;
      panasonicModel.Suspended             = false;
      panasonicModel.CashOut               = false;
      panasonicModel.Cash                  = false;
      panasonicModel.TimeZoneOfET          = Now();
      panasonicModel.TimeZoneOfST          = Now();
      panasonicModel.DayLightTimeOfET      = Now();
      panasonicModel.DayLightTimeOfST      = Now();
      panasonicModel.StartTime             = Now();
      panasonicModel.EndTime               = Now();
      panasonicModel.LastReceipt           = receipt;

      if(paymentTransaction.Money.RoundedGrandTotal < 0)
      {
            panasonicModel.RefundAmount = paymentTransaction.Money.RoundedGrandTotal;
            panasonicModel.Purchase = false;
      }
      else
      {
            panasonicModel.RefundAmount = 0;
            panasonicModel.Purchase = true;
      }
      for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
      {
            TPayment *SubPayment = paymentTransaction.PaymentGet(i);
            if (SubPayment->GetPay() != 0)
            {
                if(SubPayment->Properties == 4097)
                {
                    panasonicModel.TenderCash = RoundToNearest(SubPayment->GetPayTendered(),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
                    panasonicModel.Cash = true;
                }
                else if(SubPayment->Properties == 13)
                {
                    panasonicModel.CreditCard = true;
                }
                else if(SubPayment->Properties == 1)
                {
                    panasonicModel.Cheque = true;
                }
                else if(SubPayment->Properties == 15)
                {
                    panasonicModel.EFTPOS = true;
                    panasonicModel.CashOut = true;
                }
            }
      }

      TfrmDBPanasonic *frmDBPanasonic = new TfrmDBPanasonic(Screen->ActiveForm);
      //posting data to Sql server
      frmDBPanasonic->SendDataToServer(panasonicModel);
      delete frmDBPanasonic;

}
//--------------------------------------------------------------------------------
void TPanasonicAdapter::ConvertTransactionInfoToPanasonicItemList(TPaymentTransaction &paymentTransaction, LongInt arcBillKey)
{

     TfrmDBPanasonic *frmDBPanasonic = new TfrmDBPanasonic(Screen->ActiveForm);
     TPanasonicItemList itemList;

      //For inserting items in titemlist Table
      for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
      {
            TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);
            itemList.ProductCode                = Order->ThirdPartyCode;
            itemList.ProductDescription         = Order->Item + " " + Order->Size;
            itemList.InternalTransactionId      = arcBillKey;
            itemList.UnitPrice                  = Order->HappyHour ? Order->PriceLevel1 : Order->PriceLevel0;
            itemList.Quantity                   = double(Order->GetQty());
            itemList.Amount                     = itemList.Quantity*itemList.UnitPrice;
            itemList.Weight                     = Order->WeightedPrice.WeightedItem ?double(Order->GetQty()) : 0;
            itemList.PriceInquiry               = false;
            itemList.Void                       = Order->OrderType == 2 ? true : false;
            itemList.StaffPurchase              = false;
            itemList.Refund                     = itemList.Quantity > 0 ? false : true;
            itemList.TrainingMode               = false;

            frmDBPanasonic->InsertItemsToTItemList(itemList);

            for (int subOrderIndex = 0; subOrderIndex < Order->SubOrders->Count; subOrderIndex++)
            {
                TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[subOrderIndex];
                itemList.ProductCode                = CurrentSubOrder->ThirdPartyCode;
                itemList.ProductDescription         = CurrentSubOrder->Item + " " + CurrentSubOrder->Size;
                itemList.InternalTransactionId      = arcBillKey;
                itemList.UnitPrice                  = CurrentSubOrder->HappyHour ? CurrentSubOrder->PriceLevel1 : CurrentSubOrder->PriceLevel0;
                itemList.Quantity                   = double(CurrentSubOrder->GetQty());
                itemList.Amount                     = itemList.Quantity*itemList.UnitPrice;
                itemList.Weight                     = double(CurrentSubOrder->GetQty());
                itemList.PriceInquiry               = false;
                itemList.Void                       = Order->OrderStatus::CanceledOrder ? true : false;
                itemList.StaffPurchase              = false;
                itemList.Refund                     = itemList.Quantity > 0 ? false : true;
                itemList.TrainingMode               = false;
                frmDBPanasonic->InsertItemsToTItemList(itemList);
            }
      }
      delete frmDBPanasonic;
}
//----------------------------------------------------------------------------------------
void TPanasonicAdapter::ConvertTransactionInfoToPanasonicProduct(TPaymentTransaction &paymentTransaction)
{
    TPanasonicProduct productinfo;
    TfrmDBPanasonic *frmDBPanasonic = new TfrmDBPanasonic(Screen->ActiveForm);

      //For inserting items in titemlist Table
      for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
      {
            TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

            productinfo.ProductCode                = Order->ThirdPartyCode;
            productinfo.ProductDescription         = Order->Item + " " + Order->Size;

            frmDBPanasonic->InsertProductDetailsInToTProduct(productinfo);

            for (int subOrderIndex = 0; subOrderIndex < Order->SubOrders->Count; subOrderIndex++)
            {
                TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[subOrderIndex];

                productinfo.ProductCode                = CurrentSubOrder->ThirdPartyCode;
                productinfo.ProductDescription         = CurrentSubOrder->Item + " " + CurrentSubOrder->Size;
                frmDBPanasonic->InsertProductDetailsInToTProduct(productinfo);
            }
      }
      delete frmDBPanasonic;
}
//----------------------------------------------------------------------------------------
void TPanasonicAdapter::ConverTransactionInfoToTransactionDBServerInfo(TPaymentTransaction &paymentTransaction)
{
    TPanasonicTransactionDBServerInformation serverInfo;
    serverInfo.PosSystemType              =   "Windows";
    serverInfo.PosSystemName              =   TDeviceRealTerminal::Instance().ID.Name;
    serverInfo.PosSystemVersion           =   GetPOSVersionInfo(paymentTransaction);
    serverInfo.TransactioDBServerType     =   "TransactionDBServer";
    serverInfo.TransactionDBServerName    =   "TransactionDBServer For " + serverInfo.PosSystemName;
    serverInfo.TransactionDBServerVersion =   "1.00";
    TfrmDBPanasonic *frmDBPanasonic = new TfrmDBPanasonic(Screen->ActiveForm);
    frmDBPanasonic->InsertTransactionDBServerInformation(serverInfo);
    delete frmDBPanasonic;
}
//--------------------------------------------------------------------------------------------
UnicodeString TPanasonicAdapter::GetPOSVersionInfo(TPaymentTransaction &paymentTransaction)
{
    UnicodeString versionInfo = "";
    TIBSQL *IBInternalQuery = paymentTransaction.DBTransaction.Query(paymentTransaction.DBTransaction.AddQuery());
    IBInternalQuery->Close();
    try
	{
        IBInternalQuery->SQL->Text = "SELECT   MAX(a.VERSION_NUMBER) FROM VERSIONHISTORY a ";
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
            versionInfo = IBInternalQuery->Fields[0]->AsString;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return versionInfo;
}
