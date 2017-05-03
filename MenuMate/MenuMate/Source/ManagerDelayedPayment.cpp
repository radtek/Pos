//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerDelayedPayment.h"
#include "DBTab.h"
#include "DBTables.h"
#include "GlobalSettings.h"
#include "DBOrder.h"
#include "enumTab.h"
#include "enum.h"
#include "Invoice.h"
#include "ItemComplete.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TManagerDelayedPayment::TManagerDelayedPayment()
{
    //
}

void TManagerDelayedPayment::MoveOrderToTab(Database::TDBTransaction &DBTransaction,TSaveOrdersTo &inOrderContainer)
{
    try
    {
        AnsiString InvoiceNumber = Invoice->GetNextInvoiceNumber(DBTransaction,RegularSale);;
        AnsiString TabName = TGlobalSettings::Instance().ReceiptNumberLabel + InvoiceNumber;
        //Create Tab
        int TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
        TDBTab::SetTabType(DBTransaction, TabKey, TabDelayedPayment);
        TDBTab::SetTabName(DBTransaction, TabKey, TabName);
        //Code here
        inOrderContainer.Location["TabKey"] = TabKey;
        inOrderContainer.Location["TMMTabType"] = TabDelayedPayment;
        inOrderContainer.Location["TMMDisplayMode"] = eTabs;
        inOrderContainer.Location["ContainerName"] = TabName;
        inOrderContainer.Location["TabName"] = TabName;
        inOrderContainer.Location["PartyName"] = "";
        inOrderContainer.Location["SelectedSeat"] = 0;
        inOrderContainer.Location["SelectedTable"] = 0;
        inOrderContainer.Location["RoomNumber"] = 0;
        inOrderContainer.Location["DelayedInvoiceNumber"] = InvoiceNumber;
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

// ---------------------------------------------------------------------------

void TManagerDelayedPayment::MoveOrderToTab(TPaymentTransaction &PaymentTransaction,bool IsTransferFromTable)
{
	AnsiString InvoiceNumber = Invoice->GetNextInvoiceNumber(PaymentTransaction.DBTransaction,RegularSale);
    PaymentTransaction.InvoiceNumber =  InvoiceNumber;
	AnsiString TabName = TGlobalSettings::Instance().ReceiptNumberLabel + InvoiceNumber;
	//Create Tab
	int TabKey = TDBTab::GetOrCreateTab(PaymentTransaction.DBTransaction, 0);
	TDBTab::SetTabType(PaymentTransaction.DBTransaction, TabKey, TabDelayedPayment);
	TDBTab::SetTabName(PaymentTransaction.DBTransaction, TabKey, TabName);
    int userKey = TDeviceRealTerminal::Instance().User.ContactKey;
    TDateTime transferTime = Now();
    AnsiString SourceName = "";
    for (int CurrentIndex = 0; CurrentIndex < PaymentTransaction.Orders->Count; CurrentIndex++)
	{
            TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[CurrentIndex]);
            int sourceTabeNumber = Order->TableNo;
            SourceName =  Order->TabName;
            Order->TabKey = TabKey;
            Order->TableNo = 0;
            Order->TabContainerName = TabName;
            Order->TabName = TabName;
            Order->TabType = TabDelayedPayment;
            Order->DelayedInvoiceNumber = InvoiceNumber;
            for (int i = 0; i < Order->SubOrders->Count; i++)
            {
                TItemComplete *SubOrder = (TItemComplete*)(Order->SubOrders->Items[i]);
                SubOrder->TabKey = TabKey;
                SubOrder->TableNo = 0;
                SubOrder->TabContainerName = TabName;
                SubOrder->TabName = TabName;
                SubOrder->TabType = TabDelayedPayment;
                SubOrder->DelayedInvoiceNumber = InvoiceNumber;
            }

            if(IsTransferFromTable)
            {
               SourceName = TDBTables::GetTableName(PaymentTransaction.DBTransaction,sourceTabeNumber);
            }
            TDBOrder::UpdateOrderForDelayedPayment(PaymentTransaction.DBTransaction,Order,SourceName,TabKey,TabName,InvoiceNumber,userKey,transferTime);
	}
}

bool TManagerDelayedPayment::IsDelayedPayment(TPaymentTransaction &PaymentTransaction)
{
    AnsiString invoiceNumber = "";
    if(PaymentTransaction.Orders->Count > 0)
     {
        TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
        invoiceNumber = Order->DelayedInvoiceNumber;
     }
    if(invoiceNumber == "" || invoiceNumber == "Undefined")
       return false;
    else
       return true;
}
